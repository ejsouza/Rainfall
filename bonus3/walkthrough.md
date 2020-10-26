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
Following there's another call to `fread()` to copy `0x41 bytes` into another buffer, next the program `fclose()` the file it opened earlier.<br>
`strcmp()` is called to compare if what we entered as argument is equal to the *flag* the program read into the buferr<br>
*Whaaait, whaat?* <br>
*But how in god's name we could know the value of the flag and that this flag has a starting number large enough to not set the `\0` in the middle of the flag?* <br>
*__There's a taste of fear, When the henchmen call, Iron fist to claim it all__* <br>
