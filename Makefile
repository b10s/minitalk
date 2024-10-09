# diff btw = and := ?
CC := cc
CFLAGS := -Wall -Wextra -Werror

all: server client

# what are explicit targets like .o .c?
client:
	$(CC) $(CFLAGS)	client.c client_helper.c -o client

re: fclean all

fclean:
	rm -rf client server
