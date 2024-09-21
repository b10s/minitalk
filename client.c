#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

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

int main(void) {
	printf("hi from client\n");

	char *buf;
	int size;

	size = 3;
	buf = malloc(size);

	char b;

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


	//Q: why such type is needed at all? why not to use just long?

	//Q: from man pid_t: where the width of pid_t is  no  greater  than  the width of the type longl
	// why width? what is width? size?
	pid_t	pid;

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
