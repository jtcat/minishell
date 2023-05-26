/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 02:13:40 by joaoteix          #+#    #+#             */
/*   Updated: 2023/05/26 01:02:16 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <limits.h>
#include <utils.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#define ERR_CMD 127

void	close_pipes(int	*pipes, int pipe_n)
{
	int	*pipe;

	while (pipe_n-- > 0)
	{
		pipe = pipes + 2 * pipe_n;
		close(pipe[0]);
		close(pipe[1]);
		pipes++;
	}
}

void	str_concat(char **dst_ref, char *src)
{
	char	*tmp;

	tmp = ft_strjoin(*dst_ref, src);
	free(src);
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
char	*expand_word(t_scontext *ctx, char *word_ref)
{
	char	*cursor;
	char	*word_start;
	char	*expansion;

	cursor = word_ref;
	word_start = word_ref;
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
	return (expansion);
}

int	try_file_redir(t_scontext *ctx, char *fname_ref, int redir_from)
{
	char * const	exp_filename = expand_word(ctx, fname_ref);
	const int redir_to = open(exp_filename, 0, O_RDONLY);

	free(exp_filename);
	if (redir_to < 0)
	{
		perror(MSH_ERR_PFIX);
		return (-1);
	}
	dup2(redir_to, redir_from);
	close(redir_to);
	return (redir_to);
}

int	apply_redirs(t_scontext *ctx, t_cmd *cmd, int *pipes, int pipe_i)
{
	int * const	ppipe = pipes + 2 * pipe_i;

	if (pipe_i > 0)
	{
		dup2(ppipe[0], STDIN_FILENO);
		close(ppipe[0]);
	}
	if (pipe_i >= 0 && pipe_i )
	{
		dup2(ppipe[1], STDOUT_FILENO);
		close(ppipe[1]);
	}
	if (cmd->red_in)
		return (try_file_redir(ctx, cmd->red_in, STDIN_FILENO)
				< 0);
	if (cmd->red_out)
		return (try_file_redir(ctx, cmd->red_out, STDOUT_FILENO)
			< 0);
	return (0);
}

bool	resolve_cmd(t_scontext *ctx, char **cmd_path_ref)
{
	char	**path_dirs;
	char	**path_iter;
	char	*cmd_suffix;
	char	*tmp_path;

	if (ft_strchr(*cmd_path_ref, '/'))
		return (!access(*cmd_path_ref, X_OK));
	path_dirs = ft_split(sctx_getenv(ctx, "PATH"), ':');
	if (!path_dirs)
	{
		ft_dprintf(STDERR_FILENO, MSH_ERR_PFIX "%s: " MSH_FILE_ERR_MSG "\n", *cmd_path_ref);						
		return (false);
	}
	path_iter = path_dirs;
	cmd_suffix = ft_strjoin("/", *cmd_path_ref);
	while (*path_iter)
	{
		tmp_path = ft_strjoin(*(path_iter++), cmd_suffix);
		if (!access(tmp_path, X_OK))
		{
			free(cmd_suffix);
			*cmd_path_ref = tmp_path;
			return (true);
		}
		free(tmp_path);
	}	
	ft_dprintf(STDERR_FILENO, MSH_ERR_PFIX "%s: " MSH_CMD_NFOUND_ERR "\n", *cmd_path_ref);						
	free(cmd_suffix);
	free_ptrarr((void **)path_dirs, free);
	return (false);
}

// Returns string array with every argument expanded
char	**expand_args(t_scontext *ctx, t_cmd *cmd)
{
	t_list	*arg_iter;
	char	**args;
	int		arg_i;

	args = malloc(sizeof(char *) * (cmd->arg_n + 1));
	args[cmd->arg_n] = NULL;
	args[0] = cmd->args->content;
	arg_iter = cmd->args->next;
	arg_i = 1;
	while (arg_iter)
	{
		args[arg_i++] = expand_word(ctx, (char *)arg_iter->content);
		arg_iter = arg_iter->next;
	}
	return (args);
}

bool	try_exec_builtin(t_scontext *ctx, t_cmd	*cmd)
{
	const char	*cmd_name = (char *)cmd->args->content;
	char	**args;
	int		(*builtin_func)(t_scontext *, char **);

	if (!ft_strcmp(cmd_name, "echo"))
		builtin_func = echo_cmd;
	else if (!ft_strcmp(cmd_name, "pwd"))
		builtin_func = pwd_cmd;
	else if (!ft_strcmp(cmd_name, "cd"))
		builtin_func = cd_cmd;
	else if (!ft_strcmp(cmd_name, "export"))
		builtin_func = export_cmd;
	else if (!ft_strcmp(cmd_name, "unset"))
		builtin_func = unset_cmd;
	else if (!ft_strcmp(cmd_name, "env"))
		builtin_func = env_cmd;
	else if (!ft_strcmp(cmd_name, "exit"))
		builtin_func = exit_cmd;
	else
	 	return (false);
	args = expand_args(ctx, cmd);
	ctx->cmd_status = builtin_func(ctx, args + 1);
	free_ptrarr((void **)args, free);
	return (true);
}

// Argument expansion is messy right now. Trying to pass
// pipe_i == -1 means that the pipeline contains only one command
void	exec_cmd(t_cmd *cmd, t_scontext *ctx, int *pipes, int pipe_i)
{
	char	**args;
	char	*cmd_path;

	ctx->cmd_status = apply_redirs(ctx, cmd, pipes, pipe_i);
	if (ctx->cmd_status)
		return ;
	cmd->args->content = expand_word(ctx, (char *)cmd->args->content);
	cmd_path = cmd->args->content;
	if (pipe_i == -1 && try_exec_builtin(ctx, cmd))
		return ;
	if (fork() > 0)
		return (free(cmd->args->content));
	if (pipe_i == -1 || !try_exec_builtin(ctx, cmd))
		if (!resolve_cmd(ctx, &cmd_path))
		{
			sctx_destroy(ctx);
			exit(ERR_CMD);
		}
	args = expand_args(ctx, cmd);
	execve(cmd_path, args, ctx->envp);
	perror(MSH_ERR_PFIX);
	exit(EXIT_FAILURE);
	// Handle execve failure
}

int	exec_pipeline(t_scontext *ctx, t_list *cmd_lst, int pipe_n)
{
	int	ifd;
	int	ofd;
	int	*pipes;
	int	pipe_i;

	pipes = malloc(sizeof(int) * 2 * pipe_n);
	pipe_i = -1;
	while (cmd_lst)
	{
		ifd = dup(STDIN_FILENO);
		ofd = dup(STDOUT_FILENO);
		if (cmd_lst->next)
			pipe(pipes + 2 * ++pipe_i);
		exec_cmd((t_cmd *)(cmd_lst->content), ctx, pipes, pipe_i);
		dup2(ifd, STDIN_FILENO);
		dup2(ofd, STDOUT_FILENO);
		cmd_lst = cmd_lst->next;
	}
	while (wait(&ctx->cmd_status) > 0)
		;
	//if (errno != ECHILD)
	//close_pipes(pipes, pipe_n);
	free(pipes);
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
