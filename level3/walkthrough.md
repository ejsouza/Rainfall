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

Returning to the function `v()`, right after the call to `printf()` a value from *0x804988c* is moved into the register __eax__ and a comparison is
made:<br> `0x080484df <+59>:	cmp    eax,0x40`<br> if the value in the __address__ `0x080484df` is not equal to *0x40(64)* the program simply jumps to the `leave`
instruction and the function returns to the next instruction from `main()` that is the `leave` __address__ *0x08048525*.<br>Otherwise if value is equal to *0x40(64)*
