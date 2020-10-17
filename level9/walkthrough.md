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
We can see the class is composed with a *__constructor__* and three methods <br> *__setAnnotation(char \*)__*, *__operator+(N&))__* and *__operator-(N&)__* 
