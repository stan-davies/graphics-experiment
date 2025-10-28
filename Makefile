CMD = gcc
SRC = main.c util/util.c rend/rend.c rc_man/rc_man.c subject/subject.c
OBJ = $(SRC:.c=.o)
INC = -I . -I /opt/homebrew/include
LNK = -L /opt/homebrew/lib -lSDL2
OUT = ge


all : ${OBJ}
# 	clear
	${CMD} $^ ${INC} ${LNK} -Wall -pedantic -Wextra -o ${OUT}

${OBJ} : %.o: %.c
#	clear
	${CMD} ${INC} ${LNK} -Wall -pedantic -Wextra -c $^ -o $@


clean :
	@find . -type f -name '*.o' -delete
	@rm ${OUT}
