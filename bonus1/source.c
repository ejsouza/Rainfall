#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int				main(int argc, char **argv) {
	char		buffer[40];
	int			nb = atoi(argv[1]);

	if (nb <= 9) {
		memcpy(buffer, argv[2], nb * 4);
		if (nb == 0x574f4c46) {
			execl("sh", "/bin/sh", NULL);
		}
	}
	return (0);
}