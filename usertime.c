/* usertime.c
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
  
	TODO: print diagnostics on stderr
*/
#define _GNU_SOURCE
#include <dlfcn.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

/* environment variable to get the filename from */
static const char *envvar_fn = "USERTIMEFILE";

/* the file that contains the usertime to report via time()
   only used if the file is not specified in the environment */
static const char *fn_time = "usertime";

/* max length of string to read from the file */
const int MAXREADBUF = 32;

/*---------------------------------------- time 
  reads the time (in seconds since the UNIX epoch) from file
  if file is not available then we make a call to the real
  time()
*/
time_t
time(time_t *tloc)
{
	static time_t(*timefunc)(time_t *) = NULL;
	static int    initialized = 0;
	static time_t starttime;
	static time_t usertime = 0;
	static time_t firstrealtime;
	int    fd;
	time_t clock   = 0;
	time_t now     = 0;
	time_t last    = 0;
	int    readok  = 0;
	char   buf[MAXREADBUF+1];
	const char *fn;

	if(timefunc == NULL)
		timefunc = (time_t(*)(time_t *)) dlsym(RTLD_NEXT, "time");

	// we only read the time file the first time in

	if(initialized == 0)
	{
		fn = getenv(envvar_fn);
		if(fn == NULL)
			fn = fn_time;

		// TODO: deal with partial read

		fd = open(fn, O_RDONLY);
		if(fd > -1)
		{
			// TODO: read various formats, not just epoch seconds
			if(read(fd, buf, MAXREADBUF) > 0)
			{
				starttime = atol(buf);
				usertime = 1;
			}
			close(fd);
		}

		firstrealtime = timefunc(NULL);

		initialized = 1;
	} // filewasread

	// if we managed to get a valid usertime
	if(usertime == 1)
	{
		now = timefunc(NULL);
		clock =  starttime + difftime(now, firstrealtime);
	}
	else
		clock = timefunc(tloc);

	return clock;
} /* time */

/* usertime.c */
