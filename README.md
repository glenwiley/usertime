#usertime

This is a replacement for the system interface time() intended to be used via
the library interposer mechanism.  I use it to force an application to see a
specific system time, allowing me to test the way an application handles time
boundary transitions and other time related behaviors.

The "run" target in the Makefile provides an example of how to use
this - you do not link the library directly into your application,
rather you tell the OS to load the libtime.so library BEFORE the
system libs.  This replaces the real time() with our doctored time().

The setusertime utility provides a convenient means for populating your
usertime file.  usertime can set a static date/time or simply start the clock
from a specified date/time and let it run.

##Quickstart

On FreeBSD (and linux) follow these steps to see a user program run 
as though it is the current month/day/hour/minute/second in the year 2011:

```
# make -f Makefile.bsd setusertime
# ./setusertime -Y2011 usertime
# make -f Makefile.bsd run
```

##Setting the Time

The usertime clock is set by writing epoch seconds to the file "usertime" in the
current working directory or the file specified by the environment
variable "USERTIMEFILE".

The first line in the file specifies the start time for the clock, each
call to time() by your program will show the system clock to have advanced
from the start time specified in the file.

Call the setusertime binary to generate epoch seconds from more interesting
time specifications:

```
gwiley-reisende:usertime gwiley$ ./setusertime -?
USAGE: ./setusertime [-ri] [-t <HH:MM:SS>] [-Y <year>] [-m <month>] [-d <dom>] [-H <hour>] [-M <min>] [-S <sec>] [<output_file>]

Write seconds since the UNIX epoch to the file <output_file> (stdout
if no output file is specified).  The current system time is used
and elements specified via options are used to replace portions of
the system time before providing the output.

 -i            interperet the values in the time file
 -t <HH:MM:SS> replace the hours, minutes, seconds (seconds optional)
 -Y <year>     replace the year with <year> (must include century)
 -m <month>    replace the month with <month> (1-12)
 -d <dom>      replace the day of month with <dom> (1-31)
 -H <hour>     replace the hour with <hour> (24 hour time)
 -M <min>      replace the minute with <min>
 -S <sec>      replace the second with <sec>

example: to use todays date but with time set to 12:15pm
  setusertime -t 1215

If the environment variable USERTIMEFILE is set and no output file is
specified, then the file named in that variable will be used to store
the time.
```

#Building

There are a couple of different targets in the make file, the default target
will show you some options.  If you want to try to just get things rolling
then make the "run" target and hopefully you will get the library, a test
binary built and see it run.

There are no external dependencies beyond a resonable C compiler and a
reasonable implementation of make.

On FreeBSD 9.2

```
# make -f Makefile.bsd
```

On OSX 10.8:

```
# make -f Makefile.osx
```

On Solaris 10 (I am not sure this still works):
```
# make -f Makefile.solaris
```

#Running on FreeBSD or Linux

```
gwiley-reisende:usertime gwiley$ sh
sh-3.2$ export LD_PRELOAD=/Users/gwiley/usertime/libtime.so 
sh-3.2$ date
Mon Oct 28 13:45:10 EDT 2013
sh-3.2$ ./setusertime -Y 2011 usertime
sh-3.2$ ./tst
time: 1319823929
asctime: Fri Oct 28 13:45:29 2011

time: 1319823930
asctime: Fri Oct 28 13:45:30 2011

time: 1319823931
asctime: Fri Oct 28 13:45:31 2011

^C
sh-3.2$ 
```

#TODO
* initialize as soon as the program runs
* allow updates to file that trigger a re-read of the file
* set time using something other than epoch seconds
* set time directly via env. variable rather than file
* binary packages
* man page
* build on Linux

