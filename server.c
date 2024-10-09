#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include "./server.h"

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

// move to header file

t_receiver state;

int main(void) {
	// we start receiving from 0th byte and 0th bit
	state.byte = 0;
	state.bit = 0;
	state.cur_byte = 0;
	state.msg = NULL;
	state.is_receiving = 0;

	//Q: from man pid_t: where the width of pid_t is  no  greater  than  the width of the type longl
	// why width? what is width? size?

	//Q: why such type is needed at all? why not to use just long?
	pid_t	pid;

	pid = getpid();
	printf("PID: [%d]\n", pid);
	// what is it? link to function?
	// check what is returned? SIG_DFT or SOG_IGN?
	struct sigaction act_usr1;
	struct sigaction act_usr2;

	// what sa standf for? signal action?
	// why i do that? maybe i need ignore something?
	sigemptyset(&act_usr1.sa_mask);
	act_usr1.sa_sigaction = ft_sigusr1_hndlr;
	act_usr1.sa_flags = SA_SIGINFO;

	sigemptyset(&act_usr2.sa_mask);
	act_usr2.sa_sigaction = ft_sigusr2_hndlr;
	act_usr2.sa_flags = SA_SIGINFO;
	//oldHandler = signal(SIGUSR1, ft_sigusr1_hndlr);
	//TODO check for ret err
	sigaction(SIGUSR1, &act_usr1, NULL);
	//if (oldHandler == SIG_ERR)
		//return (1);
	//oldHandler = signal(SIGUSR2, ft_sigusr2_hndlr);
	sigaction(SIGUSR2, &act_usr2, NULL);
	//if (oldHandler == SIG_ERR)
		//return (1);
	for (;;){
		// where to use sleep and pause?
		usleep(500);
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
		printf("started receiving\n");
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
	(void)ucontext;
	(void)sig;
	//printf("FT SIGUSR1! [%d] from pid [%d]\n", sig, info->si_pid);
	act(1);
	//printf("kill from usr1 handler\n");
	kill(info->si_pid, SIGUSR1);
	//printf("killed from usr1 handler\n");
}

void ft_sigusr2_hndlr(int sig, siginfo_t *info, void *ucontext) {
	//printf("FT SIGUSR2! [%d] from pid [%d]\n", sig, info->si_pid);
	(void)sig;
	(void)ucontext;
	act(0);
	//printf("kill from usr2 handler\n");
	kill(info->si_pid, SIGUSR1);
	//printf("killed from usr2 handler\n");
}

