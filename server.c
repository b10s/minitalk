#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include "./server.h"

//TODO: if more than size received - say err
//TODO: use SA_RESTART in both sides

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

//TODO: make sleep and clean up if not signal for 700 usec or so


// global variable to keep state
t_receiver state;

int main(void) {
	// we start receiving from 0th byte and 0th bit
	state.byte = 0;
	state.bit = 0;
	state.cur_byte = 0;
	state.msg = NULL;

	//Q: from man pid_t: where the width of pid_t is  no  greater  than  the width of the type longl
	// why width? what is width? size?

	//Q: why such type is needed at all? why not to use just long?
	pid_t	pid;

	pid = getpid();
	printf("PID: [%d]\n", pid);
	// what is it? link to function?
	// check what is returned? SIG_DFT or SOG_IGN?
	struct sigaction usr1_handler;
	struct sigaction usr2_handler;

	// what sa standf for? signal action?
	// why i do that? maybe i need ignore something?
	sigemptyset(&usr1_handler.sa_mask);
	usr1_handler.sa_sigaction = ft_sigusr1_hndlr;
	// to get more details: who sent signal and do ACK on it
	// that is why SA_SIGINFO is needed
	usr1_handler.sa_flags = SA_SIGINFO;

	sigemptyset(&usr2_handler.sa_mask);
	usr2_handler.sa_sigaction = ft_sigusr2_hndlr;
	usr2_handler.sa_flags = SA_SIGINFO;
	//TODO check for ret err
	sigaction(SIGUSR1, &usr1_handler, NULL);
	sigaction(SIGUSR2, &usr2_handler, NULL);
	for (;;){
		pause();
	}
}

void act(int bit) {
	// if transmission ongoing
	if (state.tx == 1) {
		// if we are receiving size
		if (state.rx_size == 1) {
			rcv_bit(bit);
			// do all checks and state changes
			if (state.bit == 8) {
				state.bit = 0;
				state.size = state.size + state.cur_byte * (1<<(8*state.byte));
				state.byte++;
				//printf("cur b: [%d]\n", state.cur_byte);
				state.cur_byte = 0;
			}
			if (state.byte == sizeof(int)) {
				// finished receiving int
				state.bit = 0;
				state.byte = 0;
				printf("received size: [%d]\n", state.size);
				state.rx_size = 0;
				state.rx_msg = 1;
				// TODO err check
				state.msg = malloc(state.size + 1);
			}
		// if we are receiving message
		} else if (state.rx_msg) {
			rcv_bit(bit);
			if (state.bit == 8) {
				state.bit = 0;
				state.msg[state.byte] = state.cur_byte;
				state.byte++;
				//printf("cur b: [%d]\n", state.cur_byte);
				state.cur_byte = 0;
			}
			if (state.byte == state.size) {
				// finished receiving msg
				state.bit = 0;
				state.msg[state.byte] = '\0';
				state.byte = 0;
				printf("received msg: [%s]\n", state.msg);
				write(1, state.msg, state.size);
				clean_state();
			}
		// not receiveing msg, not receiving size
		// impossible!
		} else {
		}
	// in not receiving anything state, sleeping (pausing)
	} else {
		printf("First received signal for this session! started receiving\n");
		clean_state();
		state.tx = 1;
		state.rx_size = 1;
		rcv_bit(bit);
	}
}

// how to clean state correctly?
// 1. err detection - too big size, or some unexpected data coming
// 2. there delay in transmission, e.g. 500 us no signals (how to detect such delay?)
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
// 0 - as is; 1 - 256; 2 - 65536; 3 - 16777216

void rcv_bit(int val) {
	state.cur_byte = state.cur_byte | val<<state.bit;
	state.bit++;
}


//TODO: if pid is not good, we finish work with error
void ft_sigusr1_hndlr(int sig, siginfo_t *info, void *ucontext) {
	sleep(0);
	// just to not WARN on cc with flags - these args are not used
	(void)ucontext;
	(void)sig;
	//printf("FT SIGUSR1! [%d] from pid [%d]\n", sig, info->si_pid);
	act(1);
	//printf("sending back sigusr1 to client from usr1 handler\n");
	kill(info->si_pid, SIGUSR1);
	//printf("signal sent from usr1 handler\n");
}

void ft_sigusr2_hndlr(int sig, siginfo_t *info, void *ucontext) {
	state.client_pid = info->si_pid;
	sleep(0);
	//printf("FT SIGUSR2! [%d] from pid [%d]\n", sig, info->si_pid);
	(void)sig;
	(void)ucontext;
	act(0);
	//printf("sending back sigusr1 to client from usr2 handler\n");
	kill(info->si_pid, SIGUSR1);
	//printf("signal sent from usr2 handler\n");
}

