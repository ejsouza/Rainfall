#include <stdlib.h>
#include <string.h>
#include <stdio.h>
char			*auth = NULL;
char			*service = NULL;

int					main(void) {
	char 			buffer[128];

	while (1) {
		printf("%p %p\n", auth, service);

		fgets(buffer, 128, stdin);
		if (!strncmp(buffer, "auth ", 5)) {
			auth = malloc(4);
			if (strlen(&buffer[5]) < 30) { strcpy(auth, (&buffer[5]));}
		}
		else if (!strncmp(buffer, "reset", 5)) {free(auth);}
		else if (!strncmp(buffer, "service", 6)) {service = strdup(&buffer[7]);}
		else if (!strncmp(buffer, "login", 5)) {
			if (*(auth + 32)) {system("/bin/sh");}
			else {fwrite("Password:\n", 1, 10, stdout);}
		}
	}
	return (0);
}