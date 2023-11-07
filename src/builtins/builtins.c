/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leborges <leborges@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 14:22:48 by joaoteix          #+#    #+#             */
/*   Updated: 2023/11/07 17:10:46 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dl_list.h"
#include "libft.h"
#include <linux/limits.h>
#include <minishell.h>
#include <shell_utils.h>
#include <env.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define ERRC_CD_ARGS 127

// Builtin utilities
// Return values represent exit status

int	echo_cmd(t_shctx *ctx, char **args)
{
	const int	n_opt = *args && ft_strcmp(args[0], "-n") == 0;

	(void)ctx;
	if (n_opt)
		args++;
	while (*args)
	{
		ft_putstr_fd(*args, STDOUT_FILENO);
		args++;
		if (*args)
			ft_putchar_fd(' ', STDOUT_FILENO);
	}
	if (!n_opt)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}

int	cd_cmd(t_shctx *ctx, char **args)
{
	char		*curpath;
	char const	*home = get_var_val(ctx, "HOME");
	char		cwd[PATH_MAX];

	if (*(args + 1) != NULL)
	{
		ft_dprintf(STDERR_FILENO, MSH_ERR_PFIX "cd: too many arguments\n");
		return (1);
	}
	curpath = *args;
	if (!*args && *home)
		curpath = (char *)home;
	getcwd(cwd, PATH_MAX);
	if (chdir(curpath) != 0)
	{
		ft_dprintf(STDERR_FILENO, MSH_ERR_PFIX "cd: %s: %s\n",
			*args, strerror(errno));
		return (1);
	}
	set_var(ctx, "OLDPWD", cwd);
	set_var(ctx, "PWD", getcwd(cwd, PATH_MAX));
	return (0);
}

int	pwd_cmd(t_shctx *ctx, char **args)
{
	char	cwd[PATH_MAX];

	(void)ctx;
	(void)args;
	if (getcwd(cwd, PATH_MAX))
	{
		ft_dprintf(STDOUT_FILENO, "%s\n", cwd);
		return (0);
	}
	ft_dprintf(STDERR_FILENO, "pwd: %s\n", strerror(errno));
	return (1);
}

int	env_cmd(t_shctx *ctx, char **args)
{
	t_dlist	*iter;

	(void)args;
	iter = ctx->envp;
	while (iter)
	{
		ft_dprintf(STDOUT_FILENO, "%s\n", iter->content);
		iter = iter->next;
	}
	return (0);
}

#define ERR_EXIT_NAN "exit: non numeric argument: %s\n"
#define ERR_EXIT_ARGC "exit: too many arguments\n"

int	exit_cmd(t_shctx *ctx, char **args)
{
	unsigned char	exit_code;
	int				is_num;

	if (*args)
	{
		exit_code = ft_atoi(*args, &is_num);
		if (!is_num)
		{
			ft_dprintf(STDERR_FILENO, MSH_ERR_PFIX ERR_EXIT_NAN, *args);
			ctx->cmd_status = 2;
		}
		else if (*(args + 1) != NULL)
		{
			ft_putstr_fd(MSH_ERR_PFIX ERR_EXIT_ARGC, STDERR_FILENO);
			ctx->cmd_status = 1;
		}
		else
			ctx->cmd_status = (int)exit_code;
	}
	sctx_destroy(ctx);
	exit(ctx->cmd_status);
	return (ctx->cmd_status);
}
