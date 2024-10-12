#include "./client.h"

int main(int argc,char** argv) {
	int pid;
	char *msg;
	struct sigaction usr1_handler;
	struct sigaction usr2_handler;

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

void send_msg(char *msg, int pid) {
	unsigned long byte;
	int x;
	unsigned char b;
	int msg_size;
	int i;

	msg_size = ft_strlen(msg);

	for (byte = 0; byte < sizeof(int); byte++) {
		x = (msg_size >> (8*byte)) & 0xff;
		b = x;
		send_byte(pid, b);
	}
	for (i = 0; i < msg_size; i++)
		send_byte(pid, msg[i]);
}

void send_byte(int pid, unsigned char b) {
	int signal;
	for (short bit = 0; bit < 8; bit++) {
		if (1<<bit & b)
			signal = SIGUSR1;
		else
			signal = SIGUSR2;
		kill(pid, signal);
		sleep(1);
	}
}

void ack_handler() {
	sleep(0);
}

void err_handler() {
	sleep(0);
	err("server responded with err\n");
}

