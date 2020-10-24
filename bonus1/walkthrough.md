## bonus1
The `bonus1` will crash without arguments, so we disassebled the main to see that the program expects two arguments, the first argument will be passed to `atoi(argv[1])`
and the second one to `memcpy()` but before getting there we have some tests to pass, and if we can pass the tests a function for executing the shell will be called.
The first test will be if the number we provide as argument is less or equal to 0x9 <br>`0x08048441 <+29>:	cmp    DWORD PTR [esp+0x3c],0x9`<br> and this number will be store right after a `buffer[40]`

