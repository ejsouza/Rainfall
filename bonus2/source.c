#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int			lang = 0;

void		greetuser(char *buffer) {
	char		greetbuffer[0x48];

	if (lang == 0) {strncpy(greetbuffer, "Hello ", 6);}
	else if (lang == 1 ) {strncpy(greetbuffer, "Hyvää päivää ", 19);}
	else if (lang == 2 ) {strncpy(greetbuffer, "Goedemiddag! ", 13);}
	strcat(greetbuffer, buffer);
	puts(greetbuffer);
}

int		main(int argc, char **argv) {
	char		buffer[0x50];
	char		*local;
	
	if (argc != 3) {return (0);}
	strncpy(buffer, argv[1], 0x28);
	strncpy(&buffer[0x28], argv[2], 0x20);
	local = getenv("LANG");
	if (!memcmp(local, "fi", 0x2)) {lang = 0x1;}
	else if (!memcmp(local, "nl", 0x2)) {lang = 0x2;}
	greetuser(buffer);
	return(0);
}