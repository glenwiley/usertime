# Makefile for usertime
# $Id: Makefile,v 1.3 2001/01/02 01:30:26 gwiley Exp $

#CC     = CC
CC     = gcc
# Linux
CFLAGS = -fPIC
# Solaris
#CFLAGS = -PIC
DEBUG  = -g
SHELL  = /bin/ksh
# Solaris
#LDFLAGS = -G
LDFLAGS = -shared -Wl,-soname,libtime
# Linux
LIBLIBS=-ldl

run : libtime.so tst
	export LD_PRELOAD=$$PWD/libtime.so; ./tst

setusertime : setusertime.o
	$(CC) -o $@ $?

setusertime.o : setusertime.c
	$(CC) -c $(CFLAGS) $(DEBUG) $?

tst : tst.o
	$(CC) -o $@ $?

tst.o : tst.c
	$(CC) -c $(CFLAGS) $(DEBUG) tst.c

libtime.so : usertime.o
	$(CC) $(LDFLAGS) -o $@ $? $(LIBLIBS)

usertime.o : usertime.c
	$(CC) $(CFLAGS) $(DEBUG) -PIC -c $?

dist :
	cd ..; tar -cvf usertime/usertime.tar usertime/README usertime/Makefile usertime/usertime.c usertime/setusertime.c usertime/tst.c

clean :
	rm -f *.o core 

clobber : clean
	rm -f tst *.so setusertime usertime.tar usertime

# Makefile
