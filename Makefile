CMD = gcc
SRC = main.c util/util.c rend/rend.c util/logw/logw.c util/sdl_util/sdl_util.c gm_man/gm_man.c
OBJ = $(SRC:.c=.o)
INC = -I .
FLG = `sdl2-config --cflags --libs`
OUT = ge

all : ${OBJ}
# 	clear
	${CMD} $^ ${INC} ${FLG} -Wall -pedantic -Wextra -o ${OUT}

${OBJ} : %.o: %.c
#	clear
	${CMD} ${INC} ${FLG} -Wall -pedantic -Wextra -c $^ -o $@


clean :
	@find . -type f -name '*.o' -delete
	@rm ${OUT}
