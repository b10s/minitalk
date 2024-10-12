CC = cc
CFLAGS = -Wall -Wextra -Werror

all: server client

client:
	$(CC) $(CFLAGS)	client.c common.c ft_atoi.c -o client

server:
	$(CC) $(CFLAGS)	server.c server_sig_handling.c common.c -o server

re: fclean all

clean:
	rm -rf *.o

fclean: clean
	rm -rf client server

bonus: all

