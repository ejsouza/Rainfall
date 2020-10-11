#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void			n(void) { system( "/bin/cat /home/user/level7/.pass"); }

void			m(void) { puts("Nope"); }

int				main(int argc, char **argv) {
		char *buffer = malloc(64);
		void	(*faddr)(void) = (void*)malloc(4);

		strcpy(buffer, argv[1]);
		faddr = &m;
		(faddr)();
		return(0);
}