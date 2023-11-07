/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:47:25 by joaoteix          #+#    #+#             */
/*   Updated: 2023/11/07 17:52:05 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <unistd.h>

extern int	g_exit_val;

void	save_io(pid_t std_fds[2])
{
	std_fds[1] = dup(STDOUT_FILENO);
	std_fds[0] = dup(STDIN_FILENO);
}

void	restore_io(pid_t std_fds[2])
{
	dup2(std_fds[1], STDOUT_FILENO);
	close(std_fds[1]);
	dup2(std_fds[0], STDIN_FILENO);
	close(std_fds[0]);
}

int	stop_cmd(t_shctx *ctx, int pid)
{
	if (pid == -1)
		return (pid);
	sctx_destroy(ctx);
	exit(g_exit_val);
}
