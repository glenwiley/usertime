/* tst.c
   Glen Wiley <glen.wiley@gmail.com>

	usertime test executable
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
	time_t    now;
	char      *buf;
	struct tm *tmnow;

	while(1)
	{
		now = time(NULL);
		printf("time: %lu\n", now);

		tmnow = localtime(&now);
		buf   = asctime(tmnow);
		printf("asctime: %s\n", buf);

		sleep(1);
	}

	return 0;
} /* main */

/* end tst.c */
