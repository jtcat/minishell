/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:36:00 by joaoteix          #+#    #+#             */
/*   Updated: 2023/11/06 19:01:01 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sig.h"

extern int	g_exit_val;

void	handle_interact_int(int sig)
{
	ft_putchar_fd('\n', STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_exit_val = 128 + sig;
}

void	handle_exec_int(int sig)
{
	if (sig == SIGINT)
		ft_putchar_fd('\n', STDOUT_FILENO);
	else if (sig == SIGQUIT)
		ft_putstr_fd("Quit (core dumped)\n", STDOUT_FILENO);
	g_exit_val = 128 + sig;
}

void	bind_interact_sigs(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_interact_int);
}

void	bind_exec_sigs(void)
{
	signal(SIGQUIT, handle_exec_int);
	signal(SIGINT, handle_exec_int);
}
