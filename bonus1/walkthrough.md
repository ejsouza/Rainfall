## bonus1
The `bonus1` will crash without arguments, so we disassebled the main to see that the program expects two arguments, the first argument will be passed to `atoi(argv[1])`
and the second one to `memcpy()` but before getting there we have some tests to pass, and if we can pass the tests a function for executing the shell will be called.
The first test will be if the number we provide as argument is less or equal to 0x9 <br>`0x08048441 <+29>:	cmp    DWORD PTR [esp+0x3c],0x9`<br> and this number will be store right after a `buffer[40]` <br>` 0x0804843d <+25>:	mov    DWORD PTR [esp+0x3c],eax` <br>
Next our second parameter will be copied to the *buffer[40]* using the `memcpy()` function, we know this function has no checks, and we will be using it for our exploit. The size passed to `memcpy()` is our first parameter multiplied by 4 and right after the call to `memcpy()` there's a comparison if the number we passed as first argument and a huge number, if they are equal we get the shell, other the program jumps to the end. <br>
`0x08048478 <+84>:	cmp    DWORD PTR [esp+0x3c],0x574f4c46`<br>
Wait but what's the point? first there's a check for number to be less than 10 and then they want it to be = to *0x574f4c46 (1464814662)* ? what the heck.
To get around this we will use `memcpy()` to right enough character to change the value of the number and put this huge value in place *0x574f4c46*.<br>
I can hear you: *Hey, smarty pants! How are you gonna do that if `memcpy()` will copy the number you entered times 4 and you cannot enter a number greater than 9 and the number is 40 bytes above* ?
Me: Hey don't you remember those times working on the *libft* and when you passed a negative number for an unsigned type it will become a huge number, so that's what we are going to do here, let find the *offset*
