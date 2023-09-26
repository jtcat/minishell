/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 02:13:40 by joaoteix          #+#    #+#             */
/*   Updated: 2023/09/27 00:06:59 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "utils.h"
#include "exec.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int	exec_builtin(t_shctx *ctx, t_cmd *cmd)
{
	char		**args;
	int			(*builtin_func)(t_shctx *, char **);
	int			cmd_ret;
	int			std_fds[2];

	builtin_func = get_builtinfunc(cmd);
	expand_args(ctx, cmd, &args);
	if (ft_strcmp(*(char **)cmd->args->content, "exit"))
	{
		std_fds[1] = dup(STDOUT_FILENO);
		std_fds[0] = dup(STDIN_FILENO);
	}
	cmd_ret = builtin_func(ctx, args + 1);
	dup2(std_fds[1], STDOUT_FILENO);
	close(std_fds[1]);
	dup2(std_fds[0], STDOUT_FILENO);
	close(std_fds[0]);
	free(args);
	return (cmd_ret);
}

int	stop_cmd(t_shctx *ctx, int pid, int *exitval)
{
	if (pid == -1)
		return (pid);
	sctx_destroy(ctx);
	exit(*exitval);
}

// Argument expansion is messy right now.
//
// exitval is only used as external var by builtins running in the main process
//
// I don't think execve should fail.
// If it does, I don't know what the expected bash-like behaviour is
//
// First arg expansion will fail if it's NULL (like in '< file.txt' for example)
int	exec_cmd(t_cmd *cmd, t_shctx *ctx, int iofd[2], int piperfd, int *exitval)
{
	char	**args;
	int		pid;

	pid = -1;
	expand_word(ctx, (char **)cmd->args->content);
	if (iofd[0] > -1 || iofd[1] > -1 || !get_builtinfunc(cmd))
		pid = fork();
	if (pid > 0)
		return (pid);
	*exitval = resolve_redirs(ctx, cmd, iofd, piperfd);
	if (*exitval)
		return (stop_cmd(ctx, pid, exitval));
	if (get_builtinfunc(cmd))
	{
		*exitval = exec_builtin(ctx, cmd);
		return (stop_cmd(ctx, pid, exitval));
	}
	*exitval = resolve_cmd(ctx, (char **)cmd->args->content);
	if (*exitval)
		return (stop_cmd(ctx, pid, exitval));
	execve(*(char **)cmd->args->content,
		expand_args(ctx, cmd, &args), ctx->envp);
	free(args);
	sctx_destroy(ctx);
	*exitval = 1;
	perror(MSH_ERR_PFIX);
	return (stop_cmd(ctx, pid, exitval));
}

// Simple builtins (ones that don't fork) might
// leave the fds in the main process changed up
// after redirection
//
// Duplication and restoration of stdin and stdout
// between command execution might be necessary
int	exec_pipeline(t_shctx *ctx, t_list *cmd_lst)
{
	int	pipe_fd[2];
	int	tmp_fd;
	int	pipe_stat;
	int	last_pid;

	pipe_fd[0] = -1;
	tmp_fd = -1;
	while (cmd_lst)
	{
		if (cmd_lst->next)
			pipe(pipe_fd);
		else
			pipe_fd[1] = -1;
		last_pid = exec_cmd((t_cmd *)(cmd_lst->content), ctx,
				(int [2]){pipe_fd[1], tmp_fd}, pipe_fd[0], &pipe_stat);
		if (tmp_fd > -1)
			close(tmp_fd);
		cmd_lst = cmd_lst->next;
		if (!cmd_lst)
			break ;
		close(pipe_fd[1]);
		tmp_fd = dup(pipe_fd[0]);
		close(pipe_fd[0]);
	}
	waitpid(last_pid, &pipe_stat, 0);
	if (last_pid == -1)
		return (pipe_stat);
	if (WIFEXITED(pipe_stat))
		return (WEXITSTATUS(pipe_stat));
	if (WIFSIGNALED(pipe_stat))
		return ((unsigned char)128 + WTERMSIG(pipe_stat));
	return (WEXITSTATUS(pipe_stat));
}

void	exec_cmdlist(t_shctx *ctx, t_list *ppline_lst)
{
	t_ppline	*ppline;

	while (ppline_lst)
	{
		ppline = ((t_ppline *)(ppline_lst->content));
		if (ppline->op == lst_no_op
			|| (ppline->op == lst_and && ctx->cmd_status == 0)
			|| (ppline->op == lst_or && ctx->cmd_status > 0))
			ctx->cmd_status = exec_pipeline(ctx, ppline->cmds);
		ppline_lst = ppline_lst->next;
	}
}
