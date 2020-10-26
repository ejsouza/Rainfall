#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int						main(int argc, char **argv) {
	FILE	*file = fopen("/home/user/end/.pass", "r");
	char	buffer[136];

	if (file && argc == 0x2) {
		fread(buffer, 0x1, 0x42, file);
		buffer[0x42] = 0x0;
		buffer[atoi(argv[1])] = 0x0;
		fread(&buffer[0x42], 0x1, 0x41, file);
		fclose(file);
		if (!strcmp(buffer, argv[0x1])) {execl("/bin/sh", "sh", NULL);}
		else {puts(&buffer[0x42]);}
		return(0x0);
	}
	return(0xffffffff);
}