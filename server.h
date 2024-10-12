#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>

void rcv_bit(int val);
void clean_state();
void ft_sigusr1_hndlr();
void ft_sigusr2_hndlr();
void act(int bit);
void clean_state();
void rcv_bit(int val);
void clean_state_since_to();


typedef struct {
	int size;
	char *msg;
	int offset;
	int cur_byte;
	int test;
	int byte;
	int bit;
	int tx;
	int rx_size;
	int rx_msg;
	int client_pid;
} t_receiver;
