#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "./client.h"


//TODO: implement two types of ACKs;
// 1. when signal is received and processed without issues
// 2. when error is detected, so client should stop trans,ission and say error


// rewrite to use my own printf without buffering control and see if issue still here
int main(int argc,char** argv) {
	printf("hi from client\n");
	//char *unic = "あアあ";
	if (argc !=3) {
		//ERR
		printf("not enough arguments\n");
		return 0;
	}
	int pid = atoi(argv[1]);
	char *str = argv[2];
	int to_send = strlen(str);
	printf("string to send len: [%d]\n", to_send);

	struct sigaction usr1_handler;

	sigemptyset(&usr1_handler.sa_mask);
	usr1_handler.sa_handler = ack_handler;
	usr1_handler.sa_flags = 0;

	//TODO check for ret err
	sigaction(SIGUSR1, &usr1_handler, NULL);

	for (unsigned long byte = 0; byte < sizeof(int); byte++) {
		int x = (to_send >> (8*byte)) & 0xff;
		unsigned char b = x;
		send_byte(pid, b);
	}
	for (int i = 0; i < to_send; i++)
		send_byte(pid, str[i]);
}

void send_byte(int pid, unsigned char b) {
	for (short bit = 0; bit < 8; bit++) {
		if (1<<bit & b)
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		sleep(1);
	}
}

void ack_handler() {
	sleep(0);
}

void err_handler() {
	sleep(0);
	exit(1);
}

