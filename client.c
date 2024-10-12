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
	//printf("unic [%s]\n", unic);
	//write(1, unic, to_send);
	//write(1, "\n", 1);

	// set dummy handler to cause pause to return
	//signal(SIGUSR1, ack_handler);
	struct sigaction usr1_handler;

	// what sa standf for? signal action?
	// why i do that? maybe i need ignore something?
	sigemptyset(&usr1_handler.sa_mask);
	usr1_handler.sa_handler = ack_handler;
	usr1_handler.sa_flags = 0;

	//oldHandler = signal(SIGUSR1, ft_sigusr1_hndlr);
	//TODO check for ret err
	sigaction(SIGUSR1, &usr1_handler, NULL);

	for (unsigned long byte = 0; byte < sizeof(int); byte++) {
		int x = (to_send >> (8*byte)) & 0xff;
		unsigned char b = x;
		send_byte(pid, b);
		//for (int bit = 0; bit < 8; bit++) {
			//printf("byte: [%lu] bit: [%d]\n", byte, bit);
		//}
	}

	//oldHandler = signal(SIGUSR2, ft_sigusr2_hndlr);

	for (int i = 0; i < to_send; i++){
		// TODO sendstr with NUL byte
		//printf("sending byte [#%d: %c]\n", i, str[i]);
		send_byte(pid, str[i]);
		//displayBits(str[i]);
	}
	//char b;

	/*
	for (int i = 0; i < size; i++) {
		b = 0;
		for (int j = 0; j < 8; j++) {
			printf("byte [%d] bit [%d] val [%c]\n", i, j, b);
			b = b << 1;
			if (i == j)
				b = b | 1;
		}
		buf[i] = b;
	}

	for (int i = 0; i < size; i++) {
		displayBits(buf[i]);
	}
	*/


	//Q: why such type is needed at all? why not to use just long?

	//Q: from man pid_t: where the width of pid_t is  no  greater  than  the width of the type longl
	// why width? what is width? size?
	//pid_t	pid;
	//pid = 123;
	//printf("pid [%d]\n", pid);

	//pid = getpid();
	//printf("PID: [%d]\n", pid);
	// check what is returned? SIG_DFT or SOG_IGN?
	//oldHandler = signal(SIGUSR1, ft_sigusr1_hndlr);
	//if (oldHandler == SIG_ERR)
		//return (1);
	//sleep(1234);
}

void send_byte(int pid, unsigned char b) {
	//printf("sending byte\n");
	for (short bit = 0; bit < 8; bit++) {
		if (1<<bit & b) {
			// send bit 1
			kill(pid, SIGUSR1);
		} else {
			// send bit 0
			kill(pid, SIGUSR2);
		}
		// wait till ACK from server
		//printf("pausing..\n");
		// lol kek, sometimes I'm receiving signal here in between
		//sleep(2);
		//usleep(300);
		//printf("morning!\n");
		//usleep(800);

		// when I use 100 on my local PC it sometimes not enough
		// 1 second has 1 000 000 microseconds
		// even 900 is not enough: it fails on 
		// while true; do ./client $(pidof server) "привет как дела"; done
		// TODO: check how other people done here
		//usleep(300);
		sleep(1);
	}
}

void ack_handler() {
	//printf("ACK\n");
	sleep(0.1);
}
