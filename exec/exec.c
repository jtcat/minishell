/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 02:13:40 by joaoteix          #+#    #+#             */
/*   Updated: 2023/05/19 13:37:11 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <utils.h>

#define ERR_CMD 127

void	close_pipes(int	*pipes[2])
{
	while (*pipes)
	{
		close((*pipes)[0]);
		close((*pipes)[1]);
		(*pipes)++;
	}
	free(pipes);
}

char	*expand_word(t_scontext *ctx, char **word_ref)
{
	char	*start;
	char	*end;
	char	*expansion;

	start = *word_ref;
	end = *word_ref;
	expansion = ft_strdup("");
	while (is_wordchar(*end))
	{
		if (*end == '\'')
			ft_strjoin(expansion, ft_substr(end, 0, ft_strchr(end, '\'') - end + 1));
	}
	*word_ref = expansion;
	return (expansion);
}

int	try_file_redir(t_scontext *ctx, char **fname_ref, int redir_from)
{
	const int redir_to = open(expand_word(ctx, fname_ref), 0, O_RDONLY);

	if (redir_to < 0)
	{
		perror(MSH_ERR_PFIX);
		return (-1);
	}
	dup2(redir_to, redir_from);
	return (redir_to);
}

bool	apply_redirs(t_scontext *ctx, t_cmd *cmd, int *pipes[2], int pipe_i)
{
	int	fd_in;
	int	fd_out;

	if (pipe_i > 0)
		dup2(pipes[pipe_i][0], STDIN_FILENO);
	if (pipe_i >= 0)
		dup2(pipes[pipe_i][1], STDOUT_FILENO);
	close_pipes(pipes);
	fd_in = try_file_redir(ctx, &cmd->red_in, STDIN_FILENO);
	if (fd_in < 0)
		return (false);
	close(fd_in);
	fd_out = try_file_redir(ctx, &cmd->red_out, STDOUT_FILENO);
	if (fd_out < 0)
		return (false);
	close(fd_out);
	return (true);
}

bool	resolve_cmd(t_scontext *ctx, char **cmd_path_ref)
{
	char	**path_dirs;
	char	*temp_path;

	if (**cmd_path_ref == '.' || **cmd_path_ref == '/')
	{
		return (!access(*cmd_path_ref, X_OK));
	}
	path_dirs = ft_split(ft_strchr(sctx_getenv(ctx, "PATH"), '=') + 1, ':');
	if (!path_dirs)
	{
		ft_dprintf(STDERR_FILENO, MSH_ERR_PFIX "%s: " MSH_FILE_ERR_MSG "\n", *cmd_path_ref);						
		return (false);
	}
	else

	free_ptrarr((void **)path_dirs, free);
}

char	**expand_args(t_scontext *ctx, t_cmd *cmd)
{
	t_list	*arg_iter;
	char	**args;

	args = malloc(sizeof(char *) * (cmd->arg_n + 1));
	args[cmd->arg_n] = NULL;
	arg_iter = cmd->args->next;
	while (arg_iter)
	{
		args[cmd->arg_n] = expand_word(ctx, &get_token(&arg_iter)->str);
		arg_iter = arg_iter->next;
	}
	return (args);
}

bool	try_exec_builtin(t_scontext *ctx, t_cmd	*cmd)
{
	char	*cmd_name = get_token(&cmd->args)->str;
	char	**args = expand_args(ctx, cmd);

	if (ft_strcmp(cmd_name, "echo"))
		return (echo_cmd(args));
	else if (ft_strcmp(cmd_name, "pwd"))
		return (pwd_cmd());
	else if (ft_strcmp(cmd_name, "cd"))
		return (cd_cmd(ctx, args));
	else if (ft_strcmp(cmd_name, "export"))
		return (export_cmd(ctx, args));
	else if (ft_strcmp(cmd_name, "unset"))
		return (unset_cmd(ctx, args));
	else if (ft_strcmp(cmd_name, "env"))
		return (env_cmd(ctx));
	else if (ft_strcmp(cmd_name, "exit"))
		return (exit_cmd(ctx, args));
	free(args);
	return (false);
}

// pipe_i == -1 means that the pipeline contains only one command
void	exec_cmd(t_cmd *cmd, t_scontext *ctx, int *pipes[2], int pipe_i)
{
	int		pid;
	char	**args;

	expand_word(ctx, &get_token(&cmd->args)->str);
	if (pipe_i == -1 && try_exec_builtin(ctx, cmd))
	{
		ctx->cmd_status = 0;
		return ;
	}
	pid = fork();
	if (pid > 0)
		return ;
	if (!apply_redirs(ctx, cmd, pipes, pipe_i))
		exit(ERR_CMD);
	if (!try_exec_builtin(ctx, cmd))
		if (!resolve_cmd(ctx, &get_token(cmd->args->content)->str))
			exit(ERR_CMD);
	args = expand_args(ctx, cmd);
	execve(args[0], args, ctx->envp);
	// Handle execve failure
}

int	exec_pipeline(t_scontext *ctx, t_list *cmd_lst, int pipe_n)
{
	int	**pipes;

	pipes = malloc(sizeof(int[2]) * (pipe_n + 1));
	pipes[pipe_n] = NULL;
	while (cmd_lst)
	{
		exec_cmd((t_cmd *)(cmd_lst->content), ctx, pipes, --pipe_n);
		cmd_lst = cmd_lst->next;
	}
	while (wait(&ctx->cmd_status) > 0)
		;
	//if (errno != ECHILD)
	close_pipes(pipes);
	return (ctx->cmd_status);
}

int	exec_cmdlist(t_scontext *ctx, t_list *ppline_lst)
{
	int			last_exit;
	t_ppline	*ppline;

	last_exit = 0;
	while (ppline_lst)
	{
		ppline = ((t_ppline *)(ppline_lst->content));
		if (ppline->op == lst_no_op
				|| (ppline->op == lst_and && last_exit == 0) 
				|| (ppline->op == lst_or && last_exit > 0))
			last_exit = exec_pipeline(ctx, ppline->cmds, ppline->pipe_n);
		ppline_lst = ppline_lst->next;
	}
	return (last_exit);
}
