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
After some search  I could find this [SO-POST](https://stackoverflow.com/questions/44630262/what-do-the-assembly-instructions-seta-and-setb-do-after-repz-cmpsb/44630741) that could clarify some parts and made me understand that it was comparing a string character by charater,<br> *why bother? why not just call `strcmp`?* maybe to try to make it more complicated ? and there are also a bunch of *flags* being set in this way of doing the comparasion
