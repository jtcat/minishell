/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leborges <leborges@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 14:22:48 by joaoteix          #+#    #+#             */
/*   Updated: 2023/11/10 23:36:00 by jcat             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dl_list.h"
#include "libft.h"
#include <linux/limits.h>
#include <minishell.h>
#include <shell_utils.h>
#include "builtins.h"
#include <env.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

extern int	g_exit_val;

// Builtin utilities
// Return values represent exit status

int	echo_cmd(t_shctx *ctx, char **args)
{
	char	**iter;
	int		i;

	(void)ctx;
	iter = args;
	i = 1;
	if (*args && args[0][0] == '-')
		while (args[0][i] == 'n')
			i++;
	if (*args && (ft_strncmp(*args, "-n", 2) == 0) && !args[0][i])
		iter++;
	while (*iter)
	{
		ft_putstr_fd(*iter, STDOUT_FILENO);
		iter++;
		if (*iter)
			ft_putchar_fd(' ', STDOUT_FILENO);
	}
	if (!(*args && ft_strncmp(*args, "-n", 2) == 0 && !args[0][i]))
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}

int	cd_cmd(t_shctx *ctx, char **args)
{
	char		*curpath;
	char		cwd[PATH_MAX];

	curpath = *args;
	if (cd_val_args(ctx, args, &curpath) == 1)
		return (1);
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

#define ERR_EXIT_NAN "exit: %s: numeric argument required\n"
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
			g_exit_val = 2;
		}
		else if (*(args + 1) != NULL)
		{
			ft_putstr_fd(MSH_ERR_PFIX ERR_EXIT_ARGC, STDERR_FILENO);
			g_exit_val = 1;
			ft_putstr_fd("exit\n", STDERR_FILENO);
			return (g_exit_val);
		}
		else
			g_exit_val = (int)exit_code;
	}
	sctx_destroy(ctx);
	exit(g_exit_val);
	return (g_exit_val);
}
