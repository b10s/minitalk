#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

//TODO: replace printf to mine

int main(void) {
	//Q: why such type is needed at all? why not to use just long?

	//Q: from man pid_t: where the width of pid_t is  no  greater  than  the width of the type longl
	// why width? what is width? size?
	pid_t	pid;

	pid = getpid();
	printf("PID: [%d]\n", pid);
	// check what is returned? SIG_DFT or SOG_IGN?
	oldHandler = signal(SIGUSR1, ft_sigusr1_hndlr);
	if (oldHandler == SIG_ERR)
		return (1);
	sleep(1234);
}

void ft_sigusr1_hndlr(int sig) {
	printf("FT SIGUSR1!\n");
}
