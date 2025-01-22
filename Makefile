DIRS := src
SRCS := $(shell find $(DIRS) -name '*.c')
OBJS := $(SRCS:.c=.o)
LINK := ./UniC/unic.a -I./UniC -lm
CC := gcc
CFLAGS := -I./include -Wall -Wextra -Werror
LIB := linux_events.a

all: link $(LIB)

$(LIB): $(OBJS)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(LINK) -c $< -o $@

link:
	make -C UniC

clean:
	make -C UniC clean

fclean: clean
	make -C UniC fclean
	rm -f $(LIB)

re: fclean all

example: link $(LIB)
	$(CC) $(CFLAGS) -I./include -I./UniC $(LIB) examples/main.c -o examples/example && ./examples/example

.PHONY: all clean fclean re example
