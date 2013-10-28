#usertime

This is a replacement for the system interface time() intended to be used via
the library interposer mechanism.  I use it to force an application to see a
specific system time, allowing me to test the way an application handles time
boundary transitions and other time related behaviors.

The "run" target in the Makefile provides an example of how to use
this - you do not link the library directly into your application,
rather you tell the OS to load the libtime.so library BEFORE the
system libs.  This replaces the real time() with our doctored time().

The setusertime utility provides a convenient means for populating
your usertime file.

In order to provide a running clock, add a second line to the file
that begins with the letter 'R' (running).  When time() is called
it will read the time after the R (the start time) so that it can 
tell how much time has passed on the fudged clock.  In this case the 
reported time will be (usertime + (now - starttime)) where usertime 
is the first line in the file, now is retrieved from the original 
system time() function and startime is the second line of the file.

#TODO
* build on FreeBSD
* build on Linux

