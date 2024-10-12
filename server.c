#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include "./server.h"

t_receiver state;

int main(void) {
	struct sigaction usr1_handler;
	struct sigaction usr2_handler;
	pid_t	pid;

	state.byte = 0;
	state.bit = 0;
	state.cur_byte = 0;
	state.msg = NULL;
	pid = getpid();
	//TODO: replace with my printf or so
	printf("PID: [%d]\n", pid);
	if (sigemptyset(&usr1_handler.sa_mask) !=0 )
		err("error to set up server");
	usr1_handler.sa_sigaction = ft_sigusr1_hndlr;
	usr1_handler.sa_flags = SA_SIGINFO;
	if (sigemptyset(&usr2_handler.sa_mask) != 0)
		err("error to set up server");
	usr2_handler.sa_sigaction = ft_sigusr2_hndlr;
	usr2_handler.sa_flags = SA_SIGINFO | SA_RESTART;
	if (sigaction(SIGUSR1, &usr1_handler, NULL) != 0)
		err("error to set up server");
	if (sigaction(SIGUSR2, &usr2_handler, NULL) != 0)
		err("error to set up server");
	for (;;)
		pause();
	return 0;
}

void clean_state() {
	state.size = 0;
	if (state.msg != NULL) {
		free(state.msg);
		state.msg = NULL;
	}
	state.byte = 0;
	state.bit = 0;
	state.rx_msg = 0;
	state.rx_size = 0;
	state.tx = 0;
}

