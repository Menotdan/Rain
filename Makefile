CC=gcc
CFLAGS=-O2 -Wall -Wextra -Werror

RAINC_CSOURCES=$(shell find rainc_src/ -type f -name '*.c')
RAINC_OBJ = ${RAINC_CSOURCES:.c=.o}

TARGET_ARCH=x86_64
TARGET_OS=linux

all: rainc

rainc: $(RAINC_OBJ)
	${CC} -o rainc $(RAINC_OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -Irainc_src -c $< -o $@

clean:
	- rm $(RAINC_OBJ)
	- rm rainc