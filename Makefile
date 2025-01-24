DIRS := src
SRCS := $(shell find $(DIRS) -name '*.c')
OBJS := $(SRCS:.c=.o)
INCLUDES := -I./include -I./UniC
LINK := ./UniC/unic.a  -lm
CC := gcc
CFLAGS := -Wall -Wextra -Werror
LIB := linux_events.a

all: link $(LIB)

$(LIB): $(OBJS)
	ar rcs $@ $^
	rm -f $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

link:
	make -C UniC

clean:
	make -C UniC clean

fclean: clean
	make -C UniC fclean
	rm -f $(LIB)

re: fclean all

example: link $(LIB)
	$(CC) $(CFLAGS) $(INCLUDES) examples/*.c $(LIB) $(LINK) -o examples/example

.PHONY: all clean fclean re example
