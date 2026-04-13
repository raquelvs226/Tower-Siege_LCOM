# name of the program (Minix service)
PROG=lab3 
.PATH: ../lab2

# source code files to be compiled 
SRCS = lab3.c kbc.c utils.c timer.c

# additional compilation flags
CFLAGS += -pedantic
CFLAGS += -DLAB3
CFLAGS += -I../lab2       

# link the LCF library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile
.include <minix.lcom.mk>
