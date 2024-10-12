#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "./client.h"

int main(int argc,char** argv) {
	int pid;
	char *msg;
	int msg_size;
	struct sigaction usr1_handler;
	struct sigaction usr2_handler;
	int i;

	if (argc != 3)
		err("not enough arguments");
	// TODO replace with mine atoi, libft
	pid = atoi(argv[1]);
	msg = argv[2];
	// TODO replace with mine strlen, libft
	msg_size = strlen(msg);

	//TODO err handling
	sigemptyset(&usr1_handler.sa_mask);
	usr1_handler.sa_flags = SA_RESTART;
	usr1_handler.sa_handler = ack_handler;

	//TODO err handling
	sigemptyset(&usr2_handler.sa_mask);
	usr2_handler.sa_flags = SA_RESTART;
	usr2_handler.sa_handler = err_handler;

	//TODO check for ret err
	sigaction(SIGUSR1, &usr1_handler, NULL);
	//TODO err handling
	sigaction(SIGUSR2, &usr2_handler, NULL);

	send_size(msg_size, pid);
	for (i = 0; i < msg_size; i++)
		send_byte(pid, msg[i]);
}

void send_size(int msg_size, int pid) {
	unsigned long byte;
	int x;
	unsigned char b;

	for (byte = 0; byte < sizeof(int); byte++) {
		x = (msg_size >> (8*byte)) & 0xff;
		b = x;
		send_byte(pid, b);
	}
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
	err("server responded with err");
}

