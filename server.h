#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>

size_t	ft_strlen(const char *s);
char	*ft_itoa(int n);
void	rcv_bit(int val);
void	clean_state(void);
void	ft_sigusr1_hndlr(int sig, siginfo_t *si, void *osi);
void	ft_sigusr2_hndlr(int sig, siginfo_t *si, void *osi);
void	clean_state(void);
void	rcv_bit(int val);
void	clean_state_since_to(void);
void	err(char *msg);
void	set_sig_handlers(void);
int		rcv_size(int bit);
int		act(int bit);

typedef struct receiver
{
	char	*msg;
	int		size;
	int		offset;
	int		cur_byte;
	int		test;
	int		byte;
	int		bit;
	int		tx;
	int		rx_size;
	int		rx_msg;
	int		client_pid;
}	t_receiver;
