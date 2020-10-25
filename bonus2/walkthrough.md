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
