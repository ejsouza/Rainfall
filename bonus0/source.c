#include <unistd.h>
#include <string.h>
#include <stdio.h>

void			p(char *param1, char *param2) {
	char		buffer[4096];
	char		*tmp;

	puts(param2);
	read(0, &buffer, 4096);
	tmp = strchr(buffer, '\n');
	*tmp = '\0';
	strncpy(param1, buffer, 20);
} 

void			pp(char *buffer) {
	char		str1[20];
	char		str2[28];
	
	p(str1, " - ");
	p(str2, " - ");
	strcpy(buffer, str1);
	buffer[strlen(buffer)] = ' ';
	strcat(buffer, str2);
}

int				main(void) {
	char	buffer[42];

	pp(buffer);
	puts(buffer);
	return (0);
}