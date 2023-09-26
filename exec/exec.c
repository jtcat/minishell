/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 02:13:40 by joaoteix          #+#    #+#             */
/*   Updated: 2023/09/26 02:02:40 by joaoteix         ###   ########.fr       */
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

void	str_cat(char **dst_ref, char *src)
{
	char	*tmp;

	tmp = ft_strjoin(*dst_ref, src);
	free(src);
	free(*dst_ref);
	*dst_ref = tmp;
}

char	*expand_var(t_shctx *ctx, char *cursor, char **expansion)
{
	char const	*start = cursor;
	char		**env_i;
	int			id_len;

	if (*cursor == '?')
	{
		*expansion = ft_itoa(ctx->cmd_status);
		return (cursor + 1);
	}
	while (ft_isalnum(*cursor) || *cursor == '_')
		(cursor)++;
	if (cursor == start)
		return (NULL);
	id_len = cursor - start;
	env_i = ctx->envp;
	while (*env_i && ft_strncmp(*env_i, start, id_len) != 0)
		env_i++;
	if (env_i)
		str_cat(expansion, ft_strchr(*env_i, '=') + 1);
	return (cursor);
}

// Word unquoting and parameter expasion.
// This is garbage and barely works
char	*expand_word(t_shctx *ctx, char **word_ref)
{
	char	*cursor;
	char	*word_start;
	char	*expan;

	cursor = *word_ref;
	word_start = *word_ref;
	expan = ft_strdup("");
	while (is_wordchar(*cursor))
	{
		if (*cursor == '\'')
		{
			str_cat(&expan, ft_substr(word_start, 0, cursor - word_start));
			cursor = ft_strchr(cursor, '\'');
			str_cat(&expan, ft_substr(word_start, 1, cursor - word_start - 1));
			word_start = cursor + 1;
		}
		else if (*cursor == '"')
		{
			str_cat(&expan, ft_substr(word_start, 0, cursor - word_start));
			while (*cursor != '"')
				if (*cursor == '$')
					word_start = expand_var(ctx, cursor + 1, &expan);
		}
		else if (*cursor == '$')
			word_start = expand_var(ctx, cursor + 1, &expan);
		cursor++;
	}
	str_cat(&expan, ft_substr(word_start, 0, cursor - word_start));
	free(*word_ref);
	*word_ref = expan;
	return (*word_ref);
}

int	val_bin_path(char *path)
{
	if (access(path, F_OK))
		return (127);
	if (access(path, X_OK))
		return (126);
	return (0);
}

// Redo this, it's messy and exit statuses are missing
//
// Expected exit behaviour:
// Command not found: 127
// Command found, no exec perms: 126
// Failed expansion or redir: > 0
//
// If command has '/', skip to access validation
// Command resolution must return
// the last permission check error in case there was
// a name match and PATH has been searched through
int	resolve_cmd(t_shctx *ctx, char **cmd_path_ref)
{
	char	**path_dirs;
	char	**path_iter;
	char	*cmd_suffix;
	char	*tmp_path;
	int		lastaccess;

	if (ft_strchr(*cmd_path_ref, '/'))
		return (val_bin_path(*cmd_path_ref));
	path_dirs = ft_split(sctx_getenv(ctx, "PATH"), ':');
	if (!path_dirs)
	{
		ft_dprintf(STDERR_FILENO,
			MSH_ERR_PFIX "%s: " MSH_FILE_ERR_MSG "\n", *cmd_path_ref);
		return (127);
	}
	path_iter = path_dirs;
	cmd_suffix = ft_strjoin("/", *cmd_path_ref);
	while (*path_iter)
	{
		tmp_path = ft_strjoin(*(path_iter++), cmd_suffix);
		lastaccess = val_bin_path(tmp_path);
		if (!lastaccess)
		{
			free(cmd_suffix);
			*cmd_path_ref = tmp_path;
			return (0);
		}
		free(tmp_path);
	}	
	if (lastaccess == 127)
		ft_dprintf(STDERR_FILENO,
				MSH_ERR_PFIX "%s: " MSH_CMD_NFOUND_ERR "\n", *cmd_path_ref);
	else
		ft_dprintf(STDERR_FILENO,
				MSH_ERR_PFIX "%s: " MSH_CMD_NPERM_ERR "\n", *cmd_path_ref);
	free(cmd_suffix);
	free_ptrarr((void **)path_dirs, free);
	return (lastaccess);
}

// Returns string array with every argument expanded
char	**expand_args(t_shctx *ctx, t_cmd *cmd, char ***args)
{
	t_list	*arg_iter;
	int		i;

	*args = malloc(sizeof(char *) * (cmd->arg_n + 1));
	arg_iter = cmd->args->next;
	i = 1;
	(*args)[0] = cmd->args->content;
	(*args)[cmd->arg_n] = NULL;
	while (arg_iter)
	{
		(*args)[i++] = expand_word(ctx, arg_iter->content);
		arg_iter = arg_iter->next;
	}
	return (*args);
}

int	(*get_builtinfunc(t_cmd *cmd))(t_shctx *, char **)
{
	char *const	cmd_name = *(char **)cmd->args->content;

	if (!cmd_name)
		return (NULL);
	if (!ft_strcmp(cmd_name, "echo"))
		return (echo_cmd);
	if (!ft_strcmp(cmd_name, "pwd"))
		return (pwd_cmd);
	if (!ft_strcmp(cmd_name, "cd"))
		return (cd_cmd);
	if (!ft_strcmp(cmd_name, "export"))
		return (export_cmd);
	if (!ft_strcmp(cmd_name, "unset"))
		return (unset_cmd);
	if (!ft_strcmp(cmd_name, "env"))
		return (env_cmd);
	if (!ft_strcmp(cmd_name, "exit"))
		return (exit_cmd);
	return (NULL);
}

int	exec_builtin(t_shctx *ctx, t_cmd *cmd)
{
	char		**args;
	int			(*builtin_func)(t_shctx *, char **);
	int			cmd_ret;

	builtin_func = get_builtinfunc(cmd);
	expand_args(ctx, cmd, &args);
	cmd_ret = builtin_func(ctx, args + 1);
	free(args);
	return (cmd_ret);
}

int	file_redir(t_shctx *ctx, char *fname_ref, int red_type)
{
	char *const	exp_filename = expand_word(ctx, &fname_ref);
	const int	redir_to = open(exp_filename,
			(red_type == red_in) * (O_RDONLY)
			+ (red_type == red_out) * FILE_TRUNC
			+ (red_type == red_out_ap) * FILE_APPEND,
		  ACCESS_BITS);

	if (redir_to < 0)
	{
		perror(MSH_ERR_PFIX);
		free(exp_filename);
		return (-1);
	}
	dup2(redir_to, (red_type == red_in) * STDIN_FILENO
		+ (red_type == red_out || red_type == red_out_ap)
		* STDOUT_FILENO);
	close(redir_to);
	free(exp_filename);
	return (redir_to);
}

void	pipe_redir(int iofd[2])
{
	if (iofd[1] > 0)
	{
		dup2(iofd[1], STDIN_FILENO);
		close(iofd[1]);
	}
	if (iofd[0] > 0)
	{
		dup2(iofd[0], STDOUT_FILENO);
		close(iofd[0]);
	}
}

// File names can be expanded
// Here_doc delims cannot
int	resolve_redirs(t_shctx *ctx, t_cmd *cmd, int pipefd[2])
{
	t_list	*redir;
	int		redir_stat;
	t_token	*redir_tok;

	pipe_redir(pipefd);
	redir = cmd->redirs;
	while (redir)
	{
		redir_tok = get_token(&redir);
		if (redir_tok->type == here_doc)
			dup2(cmd->hd_fd, STDIN_FILENO);
		else if (redir_tok->type == red_out
			|| redir_tok->type == red_out_ap
			|| redir_tok->type == red_in)
		{
			redir_stat = file_redir(ctx, redir_tok->str, redir_tok->type);
			if (redir_stat != 0)
				return (redir_stat);
		}
		redir = redir->next;
	}
	return (0);
}

int	stop_cmd(t_shctx *ctx, int pid, int *exitval)
{
	if (pid > 0)
	{
		sctx_destroy(ctx);
		exit(*exitval);
	}
	return (pid);
}

// Argument expansion is messy right now.
//
// exitval is only used as external var by builtins running in the main process
//
// I don't think execve should fail.
// If it does, I don't know what exit code to return
int	exec_cmd(t_cmd *cmd, t_shctx *ctx, int iofd[2], int *exitval)
{
	char	**args;
	int		pid;

	pid = 0;
	if (iofd[0] != -1 || iofd[1] != -1 || !get_builtinfunc(cmd))
		pid = fork();
	if (pid > 0)
		return (pid);
	*exitval = resolve_redirs(ctx, cmd, iofd);
	if (*exitval)
		return (stop_cmd(ctx, pid, exitval));
	expand_word(ctx, (char **)cmd->args->content);
	if (get_builtinfunc(cmd))
	{
		*exitval = exec_builtin(ctx, cmd);
		return (stop_cmd(ctx, pid, exitval));
	}
	*exitval = resolve_cmd(ctx, (char **)cmd->args->content);
	if (*exitval)
		return (stop_cmd(ctx, pid, exitval));
	execve(cmd->args->content, expand_args(ctx, cmd, &args), ctx->envp);
	free(args);
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
			(int [2]){pipe_fd[1], tmp_fd}, &pipe_stat);
		if (tmp_fd > -1)
			close(tmp_fd);
		cmd_lst = cmd_lst->next;
		if (!cmd_lst)
			break ;
		tmp_fd = dup(pipe_fd[0]);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	waitpid(last_pid, &pipe_stat, 0);
	if (last_pid == 0)
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
