/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 23:14:59 by joaoteix          #+#    #+#             */
/*   Updated: 2023/10/23 11:36:23 by jcat             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"
#include "utils.h"
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

extern int	g_exit_val;

int		is_reg_file(const char *filename)
{
	struct	stat f_stat;
	stat(filename, &f_stat);
	return (S_ISREG(f_stat.st_mode));
}

void	handle_exec_err(t_shctx *ctx, t_cmd *cmd, char **args)
{
	(void)cmd;
	if (access(*args, F_OK))
	{
		ft_dprintf(STDERR_FILENO, MSH_ERR_PFIX "%s: command not found\n", *args);
		g_exit_val = 127;
	}
	else
	{
		ft_dprintf(STDERR_FILENO, MSH_ERR_PFIX "%s: %s\n", *args, strerror(errno));
		g_exit_val = 126;
	}
	free(args);
	sctx_destroy(ctx);
	exit(1);
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
void	resolve_cmd(t_shctx *ctx, char **cmd_path_ref)
{
	char	**path_dirs;
	char	**path_iter;
	char	*cmd_suffix;
	char	*tmp_path;

	if (ft_strchr(*cmd_path_ref, '/'))
		return ;
	path_dirs = ft_split(sctx_getenv(ctx, "PATH"), ':');
	if (!path_dirs)
		return ;
	path_iter = path_dirs;
	cmd_suffix = ft_strjoin("/", *cmd_path_ref);
	while (*path_iter)
	{
		tmp_path = ft_strjoin(*(path_iter++), cmd_suffix);
		if (is_reg_file(tmp_path))
		{
			free(cmd_suffix);
			*cmd_path_ref = tmp_path;
			return ;
		}
		free(tmp_path);
	}	
	free(cmd_suffix);
	free_ptrarr((void **)path_dirs, free);
	return ;
}

t_builtin_func	get_builtinfunc(t_cmd *cmd)
{
	const char *cmd_name;

	if (!cmd->args)
		return (NULL);
	cmd_name = *(char **)cmd->args->content;
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
