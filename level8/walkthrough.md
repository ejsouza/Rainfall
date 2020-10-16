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
