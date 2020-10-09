#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void			p(void) {
	fflush(stdout);
	// 0x080484e7 <+19>:	lea    -0x4c(%ebp),%eax
	char buffer[76];
	
	gets(buffer);
	if ((((unsigned int)(buffer + 0x50) & 0xb0000000) == 0xb0000000)) {
		printf("(%p)\n", buffer);
		exit(1);
	}
	puts(buffer);
	strdup(buffer);
}

int			main(void) {
	p();
	return (0);
}