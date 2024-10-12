#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>

void rcv_bit(int val);
void clean_state();
void ft_sigusr1_hndlr();
void ft_sigusr2_hndlr();
int act(int bit);
void clean_state();
void rcv_bit(int val);
void clean_state_since_to();
void err(char *msg);
int rcv_size(int bit);
size_t ft_strlen(const char *s);
char * ft_itoa(int n);
void set_sig_handlers();


//TODO remove unsued fields
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
