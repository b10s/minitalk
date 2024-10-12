#include "./server.h"

extern t_receiver state;

//TODO: if size is negative stop with erro
void act(int bit) {
	// if transmission ongoing
	state.flg = 0;
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

void clean_state_since_to() {
	if (state.flg == 0) {
		printf("yebabo\n");
		exit(1);
	}
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
