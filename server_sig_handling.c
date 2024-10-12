/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_sig_handling.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aenshin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 21:50:09 by aenshin           #+#    #+#             */
/*   Updated: 2024/10/12 21:50:10 by aenshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./server.h"

extern t_receiver	g_state;

void	ft_sigusr1_hndlr(int sig, siginfo_t *info, void *ucontext)
{
	int	signal;

	signal = SIGUSR1;
	g_state.client_pid = info->si_pid;
	(void)ucontext;
	(void)sig;
	g_state.res = 0;
	act(1);
	if (g_state.res != 0)
		signal = SIGUSR2;
	sleep(0);
	kill(info->si_pid, signal);
}

void	ft_sigusr2_hndlr(int sig, siginfo_t *info, void *ucontext)
{
	int	signal;

	signal = SIGUSR1;
	g_state.client_pid = info->si_pid;
	(void)sig;
	(void)ucontext;
	g_state.res = 0;
	act(0);
	if (g_state.res != 0)
		signal = SIGUSR2;
	sleep(0);
	kill(info->si_pid, signal);
}

int	act(int bit)
{
	if (g_state.tx == 1)
	{
		if (g_state.rx_size == 1)
		{
			if (rcv_size(bit) != 0)
				return (1);
		}
		else if (g_state.rx_msg)
			rcv_msg(bit);
	}
	else
	{
		g_state.tx = 1;
		g_state.rx_size = 1;
		rcv_bit(bit);
	}
	return (0);
}

void	rcv_msg(int bit)
{
	rcv_bit(bit);
	if (g_state.bit == 8)
	{
		g_state.bit = 0;
		g_state.msg[g_state.byte] = g_state.cur_byte;
		g_state.byte++;
		g_state.cur_byte = 0;
	}
	if (g_state.byte == g_state.size)
	{
		g_state.bit = 0;
		g_state.msg[g_state.byte] = '\0';
		g_state.byte = 0;
		write(1, g_state.msg, g_state.size);
		write(1, "\n", 1);
		clean_state(0);
	}
}

int	rcv_size(int bit)
{
	rcv_bit(bit);
	if (g_state.bit == 8)
	{
		g_state.bit = 0;
		g_state.size += g_state.cur_byte * (1 << (8 * g_state.byte));
		g_state.byte++;
		g_state.cur_byte = 0;
	}
	if (g_state.byte == sizeof(int))
	{
		g_state.bit = 0;
		g_state.byte = 0;
		g_state.rx_size = 0;
		g_state.rx_msg = 1;
		g_state.msg = malloc(g_state.size + 1);
		if (g_state.msg == NULL)
			clean_state(1);
	}
	if (g_state.size < 0)
		clean_state(1);
	return (0);
}
