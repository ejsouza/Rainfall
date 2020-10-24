## bonus1
The `bonus1` will crash without arguments, so we disassebled the main to see that the program expects two arguments, the first argument will be passed to `atoi(argv[1])`
and the second one to `memcpy()` but before getting there we have some tests to pass, and if we arrive a pass the tests a function for executing the shell will be called.
