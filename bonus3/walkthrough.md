## bonus3
### Hail to the king, hail to the one!

The `level3` is quite tricky! __*'Watch your tongue or have it cut from your head'*__ <br>
The program will call *fopen()* to get the flag. __*'Death is riding into town with armor'*__<br>
Next the program will check if the fopen succeed and if we entered exactly only one argument
```
 0x08048533 <+63>:	cmp    DWORD PTR [esp+0x9c],0x0
 0x0804853d <+73>:	cmp    DWORD PTR [ebp+0x8],0x
```
On success the program will proceed and call `fread()` in the opened file ` 0x08048518 <+36>:	mov    DWORD PTR [esp+0x9c],eax` then it will read `0x42 bytes` into a buffer, following the program will call `atoi(argv[1])` on the argument we gave the program, the value returned by `atoi(argv[1]` will be used as an *index* into the `buffer[atoi(argv[1])] = 0x0` *pay attention here little boy :p* <br>
