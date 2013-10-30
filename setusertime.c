/* setusertime.c
   Glen Wiley <glen.wiley@gmail.com>
   Copyright (c)2000, Glen Wiley <glen.wiley@gmail.com>
   
   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"), 
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:
   
   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.
   
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.
  
   this utility will write the seconds since the UNIX epoch based on time
	data supplied on the command line
*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

/* environment variable to get the filename from */
static const char *envvar_fn = "USERTIMEFILE";

void usage(const char *prg);
void interperet(char *fn);

/*---------------------------------------- main */
int
main(int argc, char *argv[])
{
	int       opt;
	int       retval  = 0;
	int       interp  = 0;
	char      *fn     = NULL;
	time_t    now;
	struct tm *nowtm;
	FILE      *fh;

	now   = time(NULL);
	nowtm = localtime(&now);

	while((opt = getopt(argc, argv, "?id:H:m:M:S:t:Y:")) != EOF)
	{
		switch(opt)
		{
			case 'd':
				nowtm->tm_mday = strtol(optarg, NULL, 10);
				if(nowtm->tm_mday > 31 || nowtm->tm_mday < 1)
				{
					fprintf(stderr, "error: day of month must be from 1 to 31\n");
					return ERANGE;
				}
				break;

			case 'H':
				nowtm->tm_hour = strtol(optarg, NULL, 10);
				if(nowtm->tm_hour < 0 || nowtm->tm_hour > 23)
				{
					fprintf(stderr, "error: hour must be from 0 to 23\n");
					return ERANGE;
				}
				break;

			case 'i':
				interp = 1;
				break;

			case 'm':
				nowtm->tm_mon = strtol(optarg, NULL, 10) - 1;
				if(nowtm->tm_mon < 0 || nowtm->tm_mon > 11)
				{
					fprintf(stderr, "error: month must be from 1 to 12\n");
					return ERANGE;
				}
				break;

			case 'M':
				nowtm->tm_min = strtol(optarg, NULL, 10);
				if(nowtm->tm_min < 0 || nowtm->tm_min > 59)
				{
					fprintf(stderr, "error: minutes must be from 0 to 59\n");
					return ERANGE;
				}
				break;

			case 'S':
				nowtm->tm_sec = strtol(optarg, NULL, 10);
				if(nowtm->tm_sec < 0 || nowtm->tm_sec > 59)
				{
					fprintf(stderr, "error: seconds must be from 0 to 59\n");
					return ERANGE;
				}
				break;

			case 't':
				sscanf(optarg, "%d:%d:%d", &(nowtm->tm_hour), &(nowtm->tm_min)
				 , &(nowtm->tm_sec));

				if(nowtm->tm_sec < 0 || nowtm->tm_sec > 59 
				 || nowtm->tm_min < 0 || nowtm->tm_min > 59
				 || nowtm->tm_hour < 0 || nowtm->tm_hour > 23)
				{
					fprintf(stderr, "error: second must be from 0 to 59\n");
					return ERANGE;
				}
				break;

			case 'Y':
				nowtm->tm_year = strtol(optarg, NULL, 10) - 1900;
				break;

			default:
				usage(argv[0]);
				return 0;

		} /* switch(opt) */
	} /* while((opt = getopt(argc, argv, "")) != EOF) */

	if(optind < argc)
		fn = argv[optind];
	else
		fn = getenv(envvar_fn);

	/* normally we will want to just crate/write the file,
	   but if the user specified interperet mode then we need to
		read the file and tell him what we see */
	if(!interp)
	{
		now = mktime(nowtm);

		if(fn != NULL)
			fh = fopen(fn, "w+");
		else
			fh = stdout;

		if(fh != NULL)
		{
			fprintf(fh, "%ld\n", now);

			if(fn != NULL)
				fclose(fh);
		}
		else
		{
			fprintf(stderr, "error writing time to %s, %d: %s\n", fn, errno
			 , strerror(errno));
			retval = errno;
		}
	}
	else
	{
		interperet(fn);
	}

	return retval;
} /* main */

/*---------------------------------------- usage
*/
void
usage(const char *prg)
{
	fprintf(stderr, "USAGE: %s [-ri] [-t <HH:MM:SS>] [-Y <year>] [-m <month>] [-d <dom>] [-H <hour>] [-M <min>] [-S <sec>] [<output_file>]\n\n"
	 "Write seconds since the UNIX epoch to the file <output_file> (stdout\n"
	 "if no output file is specified).  The current system time is used\n"
	 "and elements specified via options are used to replace portions of\n"
	 "the system time before providing the output.\n"
	 "\n"
	 " -i            interperet the values in the time file\n" 
	 " -t <HH:MM:SS> replace the hours, minutes, seconds (seconds optional)\n"
	 " -Y <year>     replace the year with <year> (must include century)\n"
	 " -m <month>    replace the month with <month> (1-12)\n"
	 " -d <dom>      replace the day of month with <dom> (1-31)\n"
	 " -H <hour>     replace the hour with <hour> (24 hour time)\n"
	 " -M <min>      replace the minute with <min>\n"
	 " -S <sec>      replace the second with <sec>\n"
	 "\n"
	 "example: to use todays date but with time set to 12:15pm\n"
	 "  setusertime -t 1215\n"
	 "\n"
	 "If the environment variable USERTIMEFILE is set and no output file is\n"
	 "specified, then the file named in that variable will be used to store\n"
	 "the time.\n"
	 "\n"
	 , prg);

	return;
} /* usage */

/*---------------------------------------- interperet */
void
interperet(char *fn)
{
	FILE   *fh;
	char   buf[32];
	char   *p;
	time_t clock;

	if(fn == NULL || strcmp(fn, "-") == 0)
		fh = stdin;
	else
		fh = fopen(fn, "r");

	if(fh != NULL)
	{
		if(fread(buf, 1, 32, fh) > 0)
		{
			clock = strtol(buf, NULL, 10);
			printf("clock set to %s", ctime(&clock));
		}
		fclose(fh);
	}
	
	return;
} /* interperet */

/* end setusertime.c */
