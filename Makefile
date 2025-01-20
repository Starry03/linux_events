NAME=event
CC=gcc
CFLAGS=-Wall -Wextra -Werror -g
LIB=./UniC/unic.a -I./UniC
INC=-I./include

all:
	make -C UniC
	$(CC) $(CFLAGS) main.c ./src/*.c -o $(NAME) $(LIB) $(INC)

clean:
	make -C UniC clean

fclean: clean
	make -C UniC fclean
	rm -f $(NAME)

re: fclean all

valgrind:
	sudo valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./$(NAME)
gdb:
	sudo gdb ./$(NAME)

.PHONY: all clean fclean re
