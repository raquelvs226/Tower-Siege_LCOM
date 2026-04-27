# name of the program (Minix service)
PROG = lab5
# source code files to be compiled
SRCS = lab5.c video.c kbc.c utils.c
# additional compilation flags
CFLAGS += -pedantic
# link the LCF library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf
# include LCOM's makefile
.include <minix.lcom.mk>
