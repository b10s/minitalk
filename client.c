#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "./client.h"

int main(int argc,char** argv) {
	if (argc != 3) {
		//TODO replace with mine printf
		printf("not enough arguments\n");
		return 0;
	}
	// TODO replace with mine atoi, libft
	int pid = atoi(argv[1]);
	char *str = argv[2];
	// TODO replace with mine strlen, libft
	int to_send = strlen(str);
	struct sigaction usr1_handler;
	struct sigaction usr2_handler;

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

	for (unsigned long byte = 0; byte < sizeof(int); byte++) {
		int x = (to_send >> (8*byte)) & 0xff;
		unsigned char b = x;
		send_byte(pid, b);
	}
	for (int i = 0; i < to_send; i++)
		send_byte(pid, str[i]);
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
	//TODO replace with mine printf or use write
	printf("oops\n");
	exit(1);
}

