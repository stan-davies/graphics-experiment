CMD = gcc

FILES = main.c util/util.c

INC = -I . -I /opt/homebrew/include

LINK = -L /opt/homebrew/lib -lSDL2

OUT = -o prog

all : compile
compile:
	@${CC} ${FILES} ${INC} ${LINK} ${OUT}
