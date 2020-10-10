#include <stdio.h>
#include <stdlib.h>

int			m;

void			v(void) {
	// 0x080484b6 <+18>:	mov    DWORD PTR [esp+0x4],0x200
	char		buffer[512];

	fgets(buffer, 512, stdin);
	printf(buffer);
	if (m == 64) {
		fwrite("Wait what?!\n", 1, 12, stdout);
		system("/bin/sh");
	}
}

int			main(void) {
	v();
	return (0);
}