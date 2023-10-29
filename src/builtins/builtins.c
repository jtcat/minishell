/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leborges <leborges@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 14:22:48 by joaoteix          #+#    #+#             */
/*   Updated: 2023/10/29 17:19:57 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dl_list.h"
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

void	set_var(t_shctx *ctx, const char *var, const char *val)
{
	char	*varname;
	char	*newvar;
	char	*tmp;
	t_dlist	*var_ref;

	varname = get_var_id(var);
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
		ft_dlstadd_back(&ctx->exports, ft_dlstnew(newvar));
	free(varname);
}

int	cd_cmd(t_shctx *ctx, char **args)
{
	char	*curpath;
	char	*pwd = get_var_val(ctx, "PWD");
	char	*home = get_var_val(ctx, "HOME");

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
	t_dlist	*iter = ctx->envp;
	(void)args;

	while (iter)
	{
		if (ft_strchr(iter->content, '='))
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

int	val_var_id(char *var)
{
	if (!(ft_isalpha(*var) || *var == '_'))
		return (0);
	while (*var && *var != '=' && (ft_isalnum(*var) || *var == '_'))
		var++;
	return (*var == '\0' || *var == '=');
}

// Need to validate ID, failing an assigment does not prevent others
int	export_vars(t_shctx *ctx, char **vars)
{
	t_dlist		*var_ref;
	const char	*varval;

	while (*vars)
	{
		if (val_var_id(*vars))
		{
			var_ref = get_export_ref(ctx, *vars);
			if (!var_ref)
			{
				ft_dlstadd_back(&ctx->exports, ft_dlstnew(get_var_id(*vars)));
				ctx->exports_len++;
			}
			else
			{
				free(var_ref->content);
				var_ref->content = get_var_id(*vars);
			}
			varval = ft_strchr(*vars, '=');
			if (varval)
				set_var(var_ref->content, varval, varval + 1);
		}
		else
			ft_dprintf(STDERR_FILENO, MSH_ERR_PFIX "`%s\': not a valid identifier\n", *vars);
		vars++;
	}
	return (0);
}

int	unset_cmd(t_shctx *ctx, char **var_ids)
{
	t_dlist *var_ref;
	
	while (*var_ids)
	{
		var_ref = get_var_ref(ctx, *var_ids);
		if (var_ref)
		{
			ft_dlstrmone(&ctx->envp, var_ref, free);
			ctx->envp_len--;
		}
		var_ref = get_export_ref(ctx, *var_ids);
		if (var_ref)
		{
			ft_dlstrmone(&ctx->exports, var_ref, free);
			ctx->exports_len--;
		}
		var_ids++;
	}
	return (0);
}

int	export_ls_vars(t_shctx *ctx)
{
	t_dlist		*lrgst;
	t_dlist		*dup_exp;
	t_dlist		*var;

	dup_exp = ft_dlstdup(ctx->exports);
	while (dup_exp)
	{
		lrgst = NULL;
		var = dup_exp;
		while (var)
		{
			if (!lrgst || ft_strcmp(var->content, lrgst->content) < 0)
				lrgst = var;
			var = var->next;
		}
		if (get_var_val(ctx, lrgst->content))
			ft_dprintf(STDOUT_FILENO, "declare -x %s=\"%s\"\n", get_var_id(lrgst->content), get_var_val(ctx, lrgst->content));
		else
			ft_dprintf(STDOUT_FILENO, "declare -x %s\n", lrgst->content);
		ft_dlstrmone(&dup_exp, lrgst, NULL);
	}
	return (0);
}

int	export_cmd(t_shctx *ctx, char **var_ids)
{
	if (*var_ids)
		return (export_vars(ctx, var_ids));
	return (export_ls_vars(ctx));
}
