#include "./client.h"

int	main(int argc, char **argv)
{
	struct sigaction	usr1_handler;
	struct sigaction	usr2_handler;
	char				*msg;
	int					pid;

	if (argc != 3)
		err("not enough arguments");
	pid = ft_atoi(argv[1]);
	msg = argv[2];
	if (sigemptyset(&usr1_handler.sa_mask) != 0)
		err("error to set up client\n");
	usr1_handler.sa_flags = SA_RESTART;
	usr1_handler.sa_handler = ack_handler;
	if (sigemptyset(&usr2_handler.sa_mask) != 0)
		err("error to set up client\n");
	usr2_handler.sa_flags = SA_RESTART;
	usr2_handler.sa_handler = err_handler;
	if (sigaction(SIGUSR1, &usr1_handler, NULL) != 0)
		err("error to set up client\n");
	if (sigaction(SIGUSR2, &usr2_handler, NULL) != 0)
		err("error to set up client\n");
	send_msg(msg, pid);
}

void	send_msg(char *msg, int pid)
{
	unsigned long	byte;
	unsigned char	b;
	int				msg_size;
	int				i;
	int				x;

	msg_size = ft_strlen(msg);
	byte = 0;
	while (byte < sizeof(int))
	{
		x = (msg_size >> (8 * byte)) & 0xff;
		b = x;
		send_byte(pid, b);
		byte++;
	}
	i = 0;
	while (i < msg_size)
		send_byte(pid, msg[i++]);
}

void	send_byte(int pid, unsigned char b)
{
	int		signal;
	short	bit;

	bit = 0;
	while (bit < 8)
	{
		if (1 << bit & b)
			signal = SIGUSR1;
		else
			signal = SIGUSR2;
		kill(pid, signal);
		sleep(1);
		bit++;
	}
}

void ack_handler() {
	sleep(0);
}

void err_handler() {
	sleep(0);
	err("server responded with err\n");
}

