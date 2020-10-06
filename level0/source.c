#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int			main(int argc, char **argv)
{
	// Trying to read with checking that's why the pogram crashes without arguments
	if (atoi(argv[1]) != 0x1a7)
		/*   The function fwrite() writes nitems objects,
		 *   each size bytes long, to the stream pointed to by stream
		 *   obtaining them from the location given by ptr.
		 */
		 fwrite("No !\n", 1, 5, stderr);
	else {
		// Get effective gourp id
		 gid_t gid = getegid();
		 // returns the real user ID of the calling proces
      		 uid_t uid = geteuid();
		 const char *path = "/bin/sh";
		 char const *array[2];
		 array[0] =  strdup(path);
		 array[1] = NULL;
		 //setresuid() sets the real user ID, the effective user ID, and the saved set-user-ID of the calling process.
      		 setresuid(uid, uid, uid);
		 setresgid(gid, gid, gid);
		 // replaces the current process image with a new process image specified by path
		 execv(path, array);
	}
	return (0);
}
