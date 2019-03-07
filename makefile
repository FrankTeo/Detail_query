

INCLUDE = -I./
LIBPATH = ./

CLIB = /pkg11/src/SCCS/lib/libcommagent.a /pkg11/src/SCCS/lib/libcli500002.a
ZLIB = /pkg11/src/SCCS/lib/libz.a

# FOR HP 64 BIT SYSTEM
#DEFINE = -DSS_64BIT_SERVER -D__COMM_AGENT__
#CFLAGS = +DA2.0W +DS2.0
#LFLAGS = 


# FOR HP 32 BIT SYSTEM
#DEFINE = #-D__COMM_AGENT__
#CFLAGS =
#LFLAGS = 


# FOR IBM 64 BIT SYSTEM
DEFINE = -D__COMM_AGENT__
CFLAGS = -q64
LFLAGS = 


COMPILE = ${CC} ${CFLAGS} ${DEFINE}
MAKEEXE = ${CC} ${CFLAGS} ${LFLAGS}


all: qdetail

qdetail:	qmain.o qret.o qdetail.o common.o commagent.o
		$(MAKEEXE) qmain.o qret.o qdetail.o common.o commagent.o -o $@ ${ZLIB} ${CLIB}

.c.o:
		$(COMPILE) -c $<

clean: 
		rm -f *.o core

