/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 02:13:40 by joaoteix          #+#    #+#             */
/*   Updated: 2023/05/24 00:04:06 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <utils.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#define ERR_CMD 127

void	close_pipes(int	(*pipes)[2])
{
	while (*pipes)
	{
		close((*pipes)[0]);
		close((*pipes)[1]);
		pipes++;
	}
	free(pipes);
}

void	str_concat(char **dst_ref, char *src)
{
	char	*tmp;

	tmp = ft_strjoin(*dst_ref, src);
	free(*dst_ref);
	*dst_ref = tmp;
}

char	*expand_var(t_scontext *ctx, char *cursor, char **expansion)
{
	char const	*start = cursor;
	char		**env_i;
	int			id_len;

	while (ft_isalnum(*cursor) || *cursor == '_')
		(cursor)++;
	if (cursor == start)
		return (NULL);
	id_len = cursor - start;
	env_i = ctx->envp;
	while (*env_i && ft_strncmp(*env_i, start, id_len) != 0)
		env_i++;
	if (env_i)
		str_concat(expansion, ft_strchr(*env_i, '=') + 1);
	return (cursor);
}

// Word unquoting and parameter expasion.
// This is garbage and does not work
char	*expand_word(t_scontext *ctx, char **word_ref)
{
	char	*cursor;
	char	*word_start;
	char	*expansion;

	cursor = *word_ref;
	word_start = *word_ref;
	expansion = ft_strdup("");
	while (is_wordchar(*cursor))
	{
		if (*cursor == '\'')
		{
			str_concat(&expansion, ft_substr(word_start, 0, cursor - word_start));
			cursor = ft_strchr(cursor, '\''); 
			str_concat(&expansion, ft_substr(word_start, 1, cursor - word_start - 1));
			word_start = cursor + 1;
		}
		else if (*cursor == '"')
		{
			str_concat(&expansion, ft_substr(word_start, 0, cursor - word_start));
			while (*cursor != '"')
				if (*cursor == '$')
					word_start = expand_var(ctx, cursor, &expansion);
		}
		else if (*cursor == '$')
			word_start = expand_var(ctx, cursor, &expansion);
		cursor++;
	}
	str_concat(&expansion, ft_substr(word_start, 0, cursor - word_start));
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

bool	apply_redirs(t_scontext *ctx, t_cmd *cmd, int (*pipes)[2], int pipe_i)
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
	char	*cmd_suffix;
	char	*tmp_path;

	if (**cmd_path_ref == '.' || **cmd_path_ref == '/')
		return (!access(*cmd_path_ref, X_OK));
	path_dirs = ft_split(ft_strchr(sctx_getenv(ctx, "PATH"), '=') + 1, ':');
	cmd_suffix = ft_strjoin("/", *cmd_path_ref);
	while (*path_dirs)
	{
		tmp_path = ft_strjoin(*(path_dirs++), cmd_suffix);
		if (!access(tmp_path, X_OK))
		{
			free(cmd_suffix);
			return (tmp_path);
		}
		free(tmp_path);
	}	
	ft_dprintf(STDERR_FILENO, MSH_ERR_PFIX "%s: " MSH_FILE_ERR_MSG "\n", *cmd_path_ref);						
	free(cmd_suffix);
	free_ptrarr((void **)path_dirs, free);
	return (NULL);
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
		args[cmd->arg_n] = expand_word(ctx, (char **)&arg_iter->content);
		arg_iter = arg_iter->next;
	}
	return (args);
}

bool	try_exec_builtin(t_scontext *ctx, t_cmd	*cmd)
{
	char	*cmd_name = (char *)cmd->args->content;
	char	**args = expand_args(ctx, cmd);

	if (ft_strcmp(cmd_name, "echo"))
		ctx->cmd_status = echo_cmd(args);
	else if (ft_strcmp(cmd_name, "pwd"))
		ctx->cmd_status = pwd_cmd();
	else if (ft_strcmp(cmd_name, "cd"))
		ctx->cmd_status = cd_cmd(ctx, args);
	else if (ft_strcmp(cmd_name, "export"))
		ctx->cmd_status = export_cmd(ctx, args);
	else if (ft_strcmp(cmd_name, "unset"))
		ctx->cmd_status = unset_cmd(ctx, args);
	else if (ft_strcmp(cmd_name, "env"))
		ctx->cmd_status = env_cmd(ctx);
	else if (ft_strcmp(cmd_name, "exit"))
		ctx->cmd_status = exit_cmd(ctx, args);
	else
	{
		free(args);
	 	return (false);
	}
	free(args);
	return (true);
}

// PROBLEM: non piped builtins dont use fork() in order to affect
// current shell (like export and exit). This messes up the redirects
// by changing the shell's std input and output. FIX THIS
//
// pipe_i == -1 means that the pipeline contains only one command
void	exec_cmd(t_cmd *cmd, t_scontext *ctx, int (*pipes)[2], int pipe_i)
{
	int		pid;
	char	**args;

	expand_word(ctx, (char **)&cmd->args->content);
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
	if (pipe_i == -1 || !try_exec_builtin(ctx, cmd))
		if (!resolve_cmd(ctx, (char **)&cmd->args->content))
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
		exec_cmd((t_cmd *)(cmd_lst->content), ctx, (int (*)[2])pipes, --pipe_n);
		cmd_lst = cmd_lst->next;
	}
	while (wait(&ctx->cmd_status) > 0)
		;
	//if (errno != ECHILD)
	close_pipes((int (*)[2])pipes);
	return (ctx->cmd_status);
}

void	exec_cmdlist(t_scontext *ctx, t_list *ppline_lst)
{
	t_ppline	*ppline;

	while (ppline_lst)
	{
		ppline = ((t_ppline *)(ppline_lst->content));
		if (ppline->op == lst_no_op
				|| (ppline->op == lst_and && ctx->cmd_status == 0) 
				|| (ppline->op == lst_or && ctx->cmd_status > 0))
			ctx->cmd_status = exec_pipeline(ctx, ppline->cmds, ppline->pipe_n);
		ppline_lst = ppline_lst->next;
	}
}
