## bonus2

The program in `bonus2` expects two arguments otherwise the program exit properly. <br> `0x08048538 <+15>:	cmp    DWORD PTR [ebp+0x8],0x3`<br>
The `main()` functions reserves space for a *buffer[0x50]* `0x08048548 <+31>:	lea    ebx,[esp+0x50]` where `40 bytes` from the first argument __argv[1]__ will be copied and
another `32 bytes` from the second arguments __argv[2]__ will be also copied right after the first <br> `40 bytes` `0x08048594 <+107>:	add    eax,0x28`. <br>
Next a call to `getenv()` wiht the the parameter `"LANG"` is made and the the value return by `getenv()` is stored at `esp+0x9c` then two calls to `memcmp()` are
made against this value, the first will check if the string is equal to *fi* if they are equal a *global* variable stored at `0x8049988` will be set to `0x1` if 
the strings doesn't match athe second call to `memcmp()` will be made this time with the string *nl* and if they are equal the *global* variable will be set `0x2`
if none the cases are true the *global* variable as a default value of `0x0`
