#include "./server.h"

extern t_receiver state;

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

void rcv_bit(int val) {
	state.cur_byte = state.cur_byte | val<<state.bit;
	state.bit++;
}

