/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leborges <leborges@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 14:22:48 by joaoteix          #+#    #+#             */
/*   Updated: 2023/05/15 11:41:22 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <minishell.h>
#include <unistd.h>

#define ERRC_CD_ARGS 127

// Builtin utilities
//
// Return values represent exit status

int	echo_cmd(char *args[])
{	
	const int	n_opt = ft_strcmp(args[0], "-n") == 0;

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

int	cd_slash_dots(char const *new_dir)
{
	return (*new_dir == '/' || !ft_strcmp(new_dir, "..") || !ft_strcmp(new_dir, ".")
		|| !ft_strncmp(new_dir, "../", 3)
		|| !ft_strncmp(new_dir, "./", 2));
}

char *get_var_id(char const *var)
{
	return (ft_substr(var, 0, ft_strchr(var, '=') - var));
}

char *sctx_getenv(t_scontext *ctx, char *const var_id)
{
	char 	**envp = ctx->envp;
	char	*envp_var_id;

	while (*envp)
	{
		envp_var_id = get_var_id(*envp);
		if (ft_strcmp(envp_var_id, var_id) == 0)
			return (*envp);
		free(envp_var_id);
		envp++;
	}
	return (NULL);
}

int	cd(t_scontext *ctx, char *new_dir)
{
	char	*temp_path;
	char	*curpath;
	char	*pwd = sctx_getenv(ctx, "PWD");
	char	*home = sctx_getenv(ctx, "HOME");

	temp_path = NULL;
	if (!new_dir && *home)
		new_dir = home;
	else if (cd_slash_dots(new_dir))
		curpath = new_dir;
	else if (*new_dir != '/')
	{
		temp_path = ft_strjoin("/", new_dir);
		curpath = ft_strjoin(pwd, temp_path);
		free(temp_path);
	}
	if (chdir(curpath) != 0)
	{
		ft_putstr_fd("cd: no such file or directory: ", STDERR_FILENO);
		ft_putstr_fd(curpath, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		return (1);
	}
	if (temp_path)
		free(curpath);
	return (0);
}

int	cd_cmd(t_scontext *ctx, char **args)
{
	if (*(args + 1) == NULL)
		return (cd(ctx, *args));
	ft_dprintf(STDERR_FILENO, MSH_ERR_PFIX "cd: too many arguments\n");
	return (ERRC_CD_ARGS);
}

int	pwd_cmd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}

int	env_cmd(t_scontext *ctx)
{
	char *const	*iter = ctx->envp;

	while (*iter)
		printf("%s\n",*(iter++));
	return (0);
}

int	exit_cmd(t_scontext *ctx, char **args)
{
	ft_putstr_fd("exit\n", STDERR_FILENO);
	exit((int));
}

void	simple_delete(void *content)
{
	free(content);
}

t_list	*find_var(char const *old_var, t_list *new_vars)
{
	char const	*old_id;
	char const	*new_id;

	while (new_vars)
	{
		old_id = get_var_id(old_var);
		new_id = get_var_id(new_vars->content);

		if (ft_strcmp(old_id, new_id) == 0)
			return (new_vars);
		new_vars = new_vars->next;
	}
	return (new_vars);
}

void	add_vars(char *envp[], char *new_envp[], t_list	*new_vars)
{
	t_list	*match_new_var;

	match_new_var = NULL;
	while (*envp)
	{
		match_new_var = find_var(*envp, new_vars);
		if (match_new_var)
			*new_envp = match_new_var->content;
		else 
			*new_envp = *envp;
		new_envp++;
		envp++;
	}
}

int	export_vars(t_scontext *ctx, t_list *vars)
{
	t_list	*vars_iter;
	char 	*var_id;
	char 	**new_envp;
	int		new_ids;

	new_ids = 0;
	vars_iter = vars;
	while (vars_iter)
	{
		var_id = get_var_id(vars_iter->content);
		if (!sctx_getenv(ctx, var_id))
			new_ids++;
		free(var_id);
		vars_iter = vars_iter->next;
	}
	ctx->envp_len = ctx->envp_len + new_ids;
	new_envp = malloc(sizeof(char *) * (ctx->envp_len + new_ids));
	add_vars(ctx->envp, new_envp, vars);
	ft_lstclear(&vars, simple_delete);
	free(ctx->envp);
	ctx->envp = new_envp;
	return (0);
}

void	remove_vars(char **envp, char **new_envp, t_list *var_ids)
{
	while (*envp)
	{
		if (!find_var(*envp, var_ids))
			*new_envp = *envp;
		envp++;
	}
}

int	unset_vars(t_scontext *ctx, t_list *var_ids)
{
	t_list	*to_unset;
	char 	**new_envp;
	int		old_ids;
	
	old_ids = 0;
	to_unset = NULL;
	while (var_ids)
	{
		if (sctx_getenv(ctx, var_ids->content))
		{
			ft_lstadd_back(&to_unset, ft_lstnew(var_ids->content));
			old_ids++;
		}
		var_ids = var_ids->next;
	}
	if (!to_unset)
		return (0);
	ctx->envp_len = ctx->envp_len - old_ids;
	new_envp = malloc(sizeof(char *) * ctx->envp_len);
	remove_vars(ctx->envp, new_envp, to_unset);
	ft_lstclear(&to_unset, simple_delete);
	free(ctx->envp);
	ctx->envp = new_envp;
	return (0);
}

int	export_cmd(t_scontext *ctx, t_list *var_ids)
{
	if (!var_ids)
		return (env_cmd(ctx));
	return (export_vars(ctx, var_ids));
}
