#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

void send_byte(int pid, unsigned char b);
void ack_handler();
void err_handler();
void err(char *msg);
void send_msg(char *msg, int pid);
size_t ft_strlen(const char *s);
char * ft_itoa(int n);
int ft_atoi(const char *str);

