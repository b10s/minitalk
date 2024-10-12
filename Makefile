# diff btw = and := ?
CC := cc
CFLAGS := -Wall -Wextra -Werror

all: server client

# what are explicit targets like .o .c?
client:
	$(CC) $(CFLAGS)	client.c common.c ft_atoi.c -o client

server:
	$(CC) $(CFLAGS)	server.c server_sig_handling.c common.c -o server

re: fclean all

fclean:
	rm -rf client server
