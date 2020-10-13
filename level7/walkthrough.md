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
Disassembling `main()` we see four calls to `malloc()`, two calls to `strcpy()` *(we know `strpcy()` is vulnerable)*, one call to `fopen()`, one call to `fgets()` and one last call to `puts()`. <br> Every call to `malloc()` is made with an `0x8 bytes` 
