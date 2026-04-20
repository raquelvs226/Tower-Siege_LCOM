# name of the program (Minix service)
PROG=lab4

# source code files to be compiled
SRCS = lab4.c kbc.c mouse.c timer.c utils.c

# additional compilation flags
CFLAGS += -pedantic
CFLAGS += -DLAB4

LIB_NAME = libmouse.a
LIB_SRCS = mouse.c
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
