void rcv_bit(int val);
void clean_state();
void ft_sigusr1_hndlr();
void ft_sigusr2_hndlr();


typedef struct {
	int is_receiving;
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
} t_receiver;
