/* tst.c
  $Id: tst.c,v 1.2 2000/12/31 20:27:38 gwiley Exp $
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

/* tst.c */
