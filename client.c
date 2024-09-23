#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

//TODO: replace printf to mine

void displayBits(unsigned char value) {
	unsigned char displayMask = 1 << 7;
	printf("%10d = ", value);
	for (unsigned char c = 1; c <= 8; ++c) {
		putchar(value & displayMask ? '1' : '0');
		value <<=1;
	}
	putchar('\n');
}

void send_byte(int pid, unsigned char b) {
	for (short bit = 0; bit < 8; bit++) {
		if (1<<bit & b) {
			// send bit 1
			kill(pid, SIGUSR1);
		} else {
			// send bit 0
			kill(pid, SIGUSR2);
		}
		usleep(100);
	}
}

int main(int argc,char** argv) {
	printf("hi from client\n");

	char *unic = "あアあ";

	char *buf;
	int size;

	if (argc !=3) {
		//ERR
		printf("not enough arguments\n");
		return 0;
	}

	int pid = atoi(argv[1]);

	size = 3;
	buf = malloc(size);

	int to_send = strlen(unic);
	printf("unic len: [%d]\n", to_send);
	printf("unic [%s]\n", unic);
	write(1, unic, to_send);
	write(1, "\n", 1);

	for (unsigned long byte = 0; byte < sizeof(int); byte++) {
		int x = (to_send >> (8*byte)) & 0xff;
		unsigned char b = x;
		send_byte(pid, b);
		for (int bit = 0; bit < 8; bit++) {
			printf("byte: [%lu] bit: [%d]\n", byte, bit);
		}
	}

	for (int i = 0; i < to_send; i++){
		// TODO sendstr with NUL byte
		//printf("sending byte [%c]\n", unic[i]);
		send_byte(pid, unic[i]);
		displayBits(unic[i]);
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

//void ft_sigusr1_hndlr(int sig) {
	//printf("FT SIGUSR1!\n");
//}
