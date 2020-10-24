## bonus0
In `bonus0` we will find three functions`p() pp() main()`






-------------------------------------------------------------
After some tests we find our *offset* as a value of `9`
```
AAAAAAAAAAAAAAAAAAAA
 -
BBBBBBBBBBCCCCCCCCCC
AAAAAAAAAAAAAAAAAAAABBBBBBBBBBCCCCCCCCCC�� BBBBBBBBBBCCCCCCCCCC��  <-- here with 10 B we still have one 42 at the address
0x43434342 in ?? () <-- the last 42 it's the 10th B

-
AAAAAAAAAAAAAAAAAAAA
 -
BBBBBBBBBCCCCCCCCCCC
AAAAAAAAAAAAAAAAAAAABBBBBBBBBCCCCCCCCCCC�� BBBBBBBBBCCCCCCCCCCC�� <-- here with 9 B the address of the $eip is set to 0x43434343 that is the C
0x43434343 in ?? () <-- offset equal 9
```
