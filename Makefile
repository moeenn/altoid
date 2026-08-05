NAME=altoid
CC=clang
CFLAGS=-Wall -Wpedantic -O3 -std=c2x
LDFLAGS=-lm -lraylib
SRC=./src/*.c
BIN=./bin/${NAME}

main:
	@${CC} ${CFLAGS} ${LDFLAGS} ${SRC} -o ${BIN}
