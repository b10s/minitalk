//TODO put other includes here
void send_byte(int pid, unsigned char b);
void ack_handler();
void err_handler();
void err(char *msg);
void send_size(int msg_size, int pid);
