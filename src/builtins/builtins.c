/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leborges <leborges@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 14:22:48 by joaoteix          #+#    #+#             */
/*   Updated: 2023/10/25 11:09:55 by jcat             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <errno.h>
#include <minishell.h>
#include <shell_utils.h>
#include <env.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define ERRC_CD_ARGS 127

// Builtin utilities
// Return values represent exit status

int	echo_cmd(t_shctx *ctx, char **args)
{	
	int	n_opt;
	(void)ctx;

	n_opt = *args && (ft_strcmp(args[0], "-n") == 0);
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

void	set_var(t_shctx *ctx, const char *varname, const char *val)
{
	char	*newvar;
	char	*tmp;
	t_list	*var_ref;

	var_ref = get_var_ref(ctx, varname);
	tmp = ft_strjoin(varname, "=");
	newvar = ft_strjoin(tmp, val);
	free(tmp);
	if (var_ref)
	{
		free(var_ref->content);
		var_ref->content = newvar;
	}
	else
		ft_lstadd_back(&ctx->envp, ft_lstnew(newvar));
}

int	cd_cmd(t_shctx *ctx, char **args)
{
	char	*curpath;
	char	*pwd = sctx_getenv(ctx, "PWD");
	char	*home = sctx_getenv(ctx, "HOME");

	if (*(args + 1) != NULL)
		ft_dprintf(STDERR_FILENO, MSH_ERR_PFIX "cd: too many arguments\n");
	return (ERRC_CD_ARGS);
	if (!*args && *home)
		curpath = home;
	if (chdir(curpath) != 0)
	{
		ft_dprintf(STDERR_FILENO, MSH_ERR_PFIX "cd: %s: %s\n", *args, strerror(errno));
		return (1);
	}
	set_var(ctx, "PWD", curpath);
	set_var(ctx, "OLD_PWD", pwd);
	return (0);
}

int	pwd_cmd(t_shctx *ctx, char **args)
{
	char	*cwd;
	(void)ctx;
	(void)args;

	cwd = getcwd(NULL, 0);
	ft_dprintf(STDOUT_FILENO, "%s\n", cwd);
	free(cwd);
	return (0);
}

int	env_cmd(t_shctx *ctx, char **args)
{
	t_list	*iter = ctx->envp;
	(void)args;

	while (iter)
	{
		ft_dprintf(STDOUT_FILENO, "%s\n", iter->content);
		iter = iter->next;
	}
	return (0);
}

int	exit_cmd(t_shctx *ctx, char **args)
{
	unsigned char	exit_code;
	int				is_num;

	if (*args)
	{
		exit_code = ft_atoi(*args, &is_num);
		if (!is_num)
		{
			ft_dprintf(STDERR_FILENO, MSH_ERR_PFIX "exit: non numeric argument: %s\n", *args);
			ctx->cmd_status = 2;
		}
		else if (*(args + 1) != NULL)
		{
			ft_putstr_fd(MSH_ERR_PFIX "exit: too many arguments\n", STDERR_FILENO);
			ctx->cmd_status = 1;
		}
		else
			ctx->cmd_status = (int)exit_code;
	}
	if (!ctx->subshell)
		ft_putstr_fd("exit\n", STDERR_FILENO);
	sctx_destroy(ctx);
	exit(ctx->cmd_status);
	return (ctx->cmd_status);
}

// Need to validate ID, failing an assigment does not prevent others
int	export_vars(t_shctx *ctx, char **vars)
{
	while (*vars)
	{
		if (val_var_id(*vars))
			set_var(ctx, *vars, get_var_val(ctx, ft_strchr(*vars, '=') + 1));
		else
		 	//print err;
		vars++;
	}
	return (0);
}

int	unset_cmd(t_shctx *ctx, char **var_ids)
{
	t_list *var_ref;
	
	while (*var_ids)
	{
		var_ref = get_var_ref(ctx, *var_ids);
		if (var_ref)
			ft_dlst_remove(var_ref, free);
		var_ref = get_export_ref(ctx, *var_ids);
		if (var_ref)
			ft_dlst_remove(var_ref, free);
		var_ids++;
	}
	return (0);
}

int	export_ls_vars(t_shctx *ctx)
{
	size_t		i;
	const char	*lst_pckd;
	const char	*lrgst_nxt;
	t_list		*var;

	lst_pckd = NULL;
	i = 0; 
	while (i < (ctx->envp_len - 1))
	{
		var = ctx->exports;
		lrgst_nxt = NULL;
		while (var)
		{
			if ((!lrgst_nxt || ft_strcmp(var->content, lrgst_nxt) < 0)
				&& (!lst_pckd || (var->content != lst_pckd && ft_strcmp(var->content, lst_pckd) >= 0)))
				lrgst_nxt = var->content;
			var = var->next;
		}
		if (get_var_val(ctx, lrgst_nxt))
			ft_dprintf(STDOUT_FILENO, "declare -x %s=\"%s\"\n", lrgst_nxt, get_var_val(ctx, lrgst_nxt));
		else
			ft_dprintf(STDOUT_FILENO, "declare -x %s\n", lrgst_nxt);
		lst_pckd = lrgst_nxt;
		i++;
	}
	return (0);
}

int	export_cmd(t_shctx *ctx, char **var_ids)
{
	if (*var_ids)
		return (export_vars(ctx, var_ids));
	return (export_ls_vars(ctx));
}
