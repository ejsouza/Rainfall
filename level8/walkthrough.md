## level8
In `level8` as opposed to the others the call to `system()` is inside the `main()` function, running the program will make it print *nil nil* and when we try to disassembly the program the disassembled code it's huge comparing to the others, so going through it with __gdb__ will show  some texts that we could think to be the inputs to pass to this program<br>
```
0x080485c1 <+93>:	mov    eax,0x8048819
(gdb) x/s 0x8048819
0x8048819:	 "auth "
```
