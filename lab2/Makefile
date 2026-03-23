# name of the program (Minix service)
PROG=lab2 

# source code files to be compiled
SRCS = lab2.c timer.c utils.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

# library configuration
LIB_NAME = libtimer.a
LIB_SRCS = timer.c utils.c
LIB_OBJS = ${LIB_SRCS:.c=.o}

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# files to be cleaned when running 'make clean'
CLEANFILES += ${LIB_NAME} ${LIB_OBJS}

# ensure the library is built before the program
all: lib

# target to build the static library
lib: ${LIB_OBJS}
	ar rcs ${LIB_NAME} ${LIB_OBJS}

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
