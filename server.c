#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

//TODO: replace printf to mine


// make gloabl struct which init
// transmission is in progress yes/no
// size of message size_t
// msg *char

// transmition start by sending 11110000
// transmition ends by sending 	00001111
// then message size as size_t in bits
// then message byte by byte
// which ending?

//TODO check some common wire transmissions encodings

// there is no transimissions in parallel?
// test on Mac and Linux

typedef struct {
	int is_receiving;
	int size;
	char *msg;
	int offset;
} t_receiver;



void ft_sigusr1_hndlr(int sig) {
	printf("FT SIGUSR1!\n");
}

void ft_sigusr2_hndlr(int sig) {
	printf("FT SIGUSR2!\n");
}

int main(void) {
	//Q: why such type is needed at all? why not to use just long?

	//Q: from man pid_t: where the width of pid_t is  no  greater  than  the width of the type longl
	// why width? what is width? size?
	pid_t	pid;

	pid = getpid();
	printf("PID: [%d]\n", pid);
	void (*oldHandler)(int); 
	// check what is returned? SIG_DFT or SOG_IGN?
	oldHandler = signal(SIGUSR1, ft_sigusr1_hndlr);
	if (oldHandler == SIG_ERR)
		return (1);
	oldHandler = signal(SIGUSR2, ft_sigusr2_hndlr);
	if (oldHandler == SIG_ERR)
		return (1);
	for (;;){
		sleep(1234);
	}
}
