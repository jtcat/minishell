/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 23:14:59 by joaoteix          #+#    #+#             */
/*   Updated: 2023/09/27 00:15:16 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"
#include "utils.h"
#include <unistd.h>
#include <errno.h>

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

t_builtin_func	get_builtinfunc(t_cmd *cmd)
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
