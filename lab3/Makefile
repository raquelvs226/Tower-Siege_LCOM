# name of the program (Minix service)
PROG=lab3 
.PATH: ../lab2

# source code files to be compiled 
SRCS = lab3.c kbc.c utils.c timer.c

# additional compilation flags
CFLAGS += -pedantic
CFLAGS += -DLAB3
CFLAGS += -I../lab2       

LIB_NAME = libkbc.a
LIB_SRCS = kbc.c
LIB_OBJS = ${LIB_SRCS:.c=.o}

# link the LCF library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf

CLEANFILES += ${LIB_NAME} ${LIB_OBJS}

all: lib

lib: ${LIB_OBJS}
	ar rcs ${LIB_NAME} ${LIB_OBJS}

# include LCOM's makefile
.include <minix.lcom.mk>
