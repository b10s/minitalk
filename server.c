/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aenshin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 21:50:03 by aenshin           #+#    #+#             */
/*   Updated: 2024/10/12 21:50:04 by aenshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./server.h"

t_receiver	g_state;

int	main(void)
{
	pid_t	pid;
	char	*pid_str;

	g_state.byte = 0;
	g_state.bit = 0;
	g_state.cur_byte = 0;
	g_state.msg = NULL;
	pid = getpid();
	pid_str = ft_itoa(pid);
	if (pid_str == NULL)
		err("failed to setup server\n");
	write(1, pid_str, ft_strlen(pid_str));
	write(1, "\n", 1);
	free(pid_str);
	set_sig_handlers();
	while (1)
		pause();
}

void	set_sig_handlers(void)
{
	struct sigaction	usr1_handler;
	struct sigaction	usr2_handler;

	if (sigemptyset(&usr1_handler.sa_mask) != 0)
		err("error to set up server\n");
	usr1_handler.sa_flags = SA_SIGINFO | SA_RESTART;
	usr1_handler.sa_sigaction = ft_sigusr1_hndlr;
	if (sigemptyset(&usr2_handler.sa_mask) != 0)
		err("error to set up server\n");
	usr2_handler.sa_flags = SA_SIGINFO | SA_RESTART;
	usr2_handler.sa_sigaction = ft_sigusr2_hndlr;
	if (sigaction(SIGUSR1, &usr1_handler, NULL) != 0)
		err("error to set up server\n");
	if (sigaction(SIGUSR2, &usr2_handler, NULL) != 0)
		err("error to set up server\n");
}

void	clean_state(int res)
{
	g_state.res = res;
	g_state.size = 0;
	if (g_state.msg != NULL)
	{
		free(g_state.msg);
		g_state.msg = NULL;
	}
	g_state.byte = 0;
	g_state.bit = 0;
	g_state.rx_msg = 0;
	g_state.rx_size = 0;
	g_state.tx = 0;
}

void	rcv_bit(int val)
{
	g_state.cur_byte = g_state.cur_byte | val << g_state.bit;
	g_state.bit++;
}
