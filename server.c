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
		err("error to set up server\n");
	usr1_handler.sa_sigaction = ft_sigusr1_hndlr;
	usr1_handler.sa_flags = SA_SIGINFO;
	if (sigemptyset(&usr2_handler.sa_mask) != 0)
		err("error to set up server\n");
	usr2_handler.sa_sigaction = ft_sigusr2_hndlr;
	usr2_handler.sa_flags = SA_SIGINFO | SA_RESTART;
	if (sigaction(SIGUSR1, &usr1_handler, NULL) != 0)
		err("error to set up server\n");
	if (sigaction(SIGUSR2, &usr2_handler, NULL) != 0)
		err("error to set up server\n");
	for (;;)
		pause();
	return 0;
}

void err(char *msg) {
	printf("err: [%s]\n", msg);
	exit(1);
}

int rcv_size(int bit) {
			rcv_bit(bit);
			if (state.bit == 8) {
				state.bit = 0;
				state.size = state.size + state.cur_byte * (1<<(8*state.byte));
				state.byte++;
				state.cur_byte = 0;
			}
			if (state.byte == sizeof(int)) {
				state.bit = 0;
				state.byte = 0;
				printf("received size: [%d]\n", state.size);
				state.rx_size = 0;
				state.rx_msg = 1;
				state.msg = malloc(state.size + 1);
				if (state.msg == NULL) {
					clean_state();
					return 1;
				}
			}
			if (state.size < 0) {
				clean_state();
				return 1;
			}
			return 0;
}

int act(int bit) {
	if (state.tx == 1) {
		if (state.rx_size == 1) {
			if (rcv_size(bit) !=0)
				return 1;
		} else if (state.rx_msg) {
			rcv_bit(bit);
			if (state.bit == 8) {
				state.bit = 0;
				state.msg[state.byte] = state.cur_byte;
				state.byte++;
				state.cur_byte = 0;
			}
			if (state.byte == state.size) {
				state.bit = 0;
				state.msg[state.byte] = '\0';
				state.byte = 0;
				write(1, state.msg, state.size);
				clean_state();
			}
		}
	} else {
		state.tx = 1;
		state.rx_size = 1;
		rcv_bit(bit);
	}
	return 0;
}

void clean_state() {
	printf("cleaning state\n");
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

void rcv_bit(int val) {
	state.cur_byte = state.cur_byte | val<<state.bit;
	state.bit++;
}


void ft_sigusr1_hndlr(int sig, siginfo_t *info, void *ucontext) {
	int signal = SIGUSR1;
	state.client_pid = info->si_pid;
	(void)ucontext;
	(void)sig;
	if (act(1) != 0)
		signal = SIGUSR2;
	sleep(0);
	kill(info->si_pid, signal);
}

void ft_sigusr2_hndlr(int sig, siginfo_t *info, void *ucontext) {
	int signal = SIGUSR1;
	state.client_pid = info->si_pid;
	(void)sig;
	(void)ucontext;
	if (act(0) != 0)
		signal = SIGUSR2;
	sleep(0);
	kill(info->si_pid, signal);
}

