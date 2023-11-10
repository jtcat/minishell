/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 02:13:40 by joaoteix          #+#    #+#             */
/*   Updated: 2023/11/10 21:40:20 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <shell_utils.h>
#include <parser.h>
#include <sig.h>
#include "exec.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

extern int	g_exit_val;

int	exec_builtin(t_shctx *ctx, t_cmd *cmd)
{
	char		**args;
	int			(*builtin_func)(t_shctx *, char **);
	int			cmd_ret;

	builtin_func = get_builtinfunc(cmd);
	args = expand_args(ctx, cmd);
	cmd_ret = builtin_func(ctx, args + 1);
	free(args);
	return (cmd_ret);
}

// Argument expansion is messy right now.
//
// First arg expansion will fail if it's NULL (like in '< file.txt' for example)
int	exec_cmd(t_cmd *cmd, t_shctx *ctx, int pipefd[2], int prevread)
{
	char	**args;
	char	**envp;
	int		pid;

	pid = -1;
	if (cmd->args)
		cmd->cmdpath = ft_strdup(expand_word(ctx, (char **)cmd->args->content, 0));
	if (prevread > -1 || pipefd[1] > -1 || !get_builtinfunc(cmd))
		pid = fork();
	if (pid > 0)
		return (pid);
	ctx->subshell = pid == 0;
	if (!resolve_redirs(ctx, cmd, pipefd, prevread) || !cmd->args)
		return (stop_cmd(ctx, pid));
	if (get_builtinfunc(cmd))
	{
		g_exit_val = exec_builtin(ctx, cmd);
		return (stop_cmd(ctx, pid));
	}
	resolve_cmd(ctx, &cmd->cmdpath);
	args = expand_args(ctx, cmd);
	envp = conv_llenvp(ctx->envp_len, ctx->envp);
	execve(cmd->cmdpath, args, envp);
	handle_exec_err(ctx, args, envp);
	return (pid);
}

void	waitexec(pid_t last_pid)
{
	waitpid(last_pid, &g_exit_val, 0);
	if (last_pid == -1)
		return ;
	if (WIFEXITED(g_exit_val))
		g_exit_val = WEXITSTATUS(g_exit_val);
	else if (WIFSIGNALED(g_exit_val))
		g_exit_val = (unsigned char)(128 + WTERMSIG(g_exit_val));
	while (wait(NULL) > 0)
		;
}

// Simple builtins (ones that don't fork) might
// leave the fds in the main process changed up
// after redirection. So duplication and
// restoration of stdin and stdout between command
// execution might be necessary (DONE)
//
// Current command execution status must be stored
// temporarily as to not overwrite global exit status
// during redirection
void	exec_pipeline(t_shctx *ctx, t_list *cmd_lst)
{
	int		pipe_fd[2];
	int		tmp_fd;
	pid_t	last_pid;

	tmp_fd = -1;
	while (cmd_lst)
	{
		pipe_fd[0] = -1;
		pipe_fd[1] = -1;
		if (cmd_lst->next)
			pipe(pipe_fd);
		save_io(ctx->std_fds);
		last_pid = exec_cmd((t_cmd *)(cmd_lst->content), ctx, pipe_fd, tmp_fd);
		restore_io(ctx->std_fds);
		if (tmp_fd > -1)
			close(tmp_fd);
		cmd_lst = cmd_lst->next;
		if (!cmd_lst)
			break ;
		close(pipe_fd[1]);
		tmp_fd = dup(pipe_fd[0]);
		close(pipe_fd[0]);
	}
	waitexec(last_pid);
}

void	exec_cmdlist(t_shctx *ctx, t_list *ppline_lst)
{
	t_ppline	*ppline;

	bind_exec_sigs();
	while (ppline_lst)
	{
		ppline = ((t_ppline *)(ppline_lst->content));
		if (ppline->op == lst_no_op
			|| (ppline->op == lst_and && g_exit_val == 0)
			|| (ppline->op == lst_or && g_exit_val > 0))
			exec_pipeline(ctx, ppline->cmds);
		ppline_lst = ppline_lst->next;
	}
	bind_interact_sigs();
}
