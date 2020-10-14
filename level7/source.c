#include<time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char			*c;

void			m(void) {printf("%s - %ld\n", c, time(0));}

int				main(int argc, char **argv) {
	int		*p_one;	
	int		*p_two;

	p_one = malloc(8);
	p_one[0] = 0x1;
	p_one[1] = malloc(8);
	p_two = malloc(8);
	p_two[0] = 0x2;	
	p_two[1] = malloc(8);
	strcpy(&p_one[1], argv[1]);
	strcpy(&p_two[1], argv[2]);
	FILE *fd = fopen("/home/user/level8/.pass", "r");
	fgets(c, 0x44, fd);
	puts("~~");
	return (0);
}