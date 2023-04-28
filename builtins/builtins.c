/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leborges <leborges@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 14:22:48 by joaoteix          #+#    #+#             */
/*   Updated: 2023/04/28 20:30:04 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

// Builtin utilities
//
// Return values represent exit status

int	echo_cmd(char *str[], int opt_n)
{	
	while (*str)
	{
		ft_putstr_fd(*str, STDOUT_FILENO);
		str++;
		if (*str)
			ft_putchar_fd(' ', STDOUT_FILENO);
	}
	if (!opt_n)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}

/*
int	cd(t_scontext *ctx, char *new_dir)
{
	char		*curpath;
	const char	*pwd = getenv("PWD");
	const char	*home = getenv("HOME");

	if (!new_dir && *home)
		new_dir = home;
	if (*new_dir == '/')
		curpath = new_dir;
}*/

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
	char const*const	*iter = ctx->envp;

	while (*iter)
		printf("%s\n",*(iter++));
	return (0);
}

void	exit_cmd(unsigned char exit_code)
{
	exit((int)exit_code);
}

void	simple_delete(void *content)
{
	free(content);
}

void	add_vars(char **envp, t_list *vars, int envlen)
{
	while (vars)
	{
		envp[envlen++] = vars->content;
		vars = vars->next;
	}
}

char const	**inter_get_var(char const *envp[], char const *const var_id)
{
	while (*envp)
	{
<<<<<<< HEAD
		if (strcmp(*envp, var_id) == 0)
			return (envp);
=======
		if (ft_strcmp(*envp, var_id) == 0)
			return (*envp);
>>>>>>> 868c44bcdd01e2a6bc7197c9ff66a5ef87dd7208
		envp++;
	}
	return (NULL);
}

char const	*sctx_get_param(t_scontext *ctx, char const *const var_id)
{
	return (*inter_get_var(ctx->svars, var_id));
}

char const	*sctx_get_var(t_scontext *ctx, char const *const var_id)
{
	char const	*ret;

	ret = *inter_get_var(ctx->envp, var_id);
	if (ret)
		return (ret);
	ret = *inter_get_var(ctx->svars, var_id);
	return (ret);
}

int	ptrarr_len(void **arr)
{
	int	len;

	len = 0;
	while (arr[len++])
		;
	return (len);
}

int	export_vars(char const **envp[], char *var_ids[])
{
	t_list	*to_export;
	char	**new_envp;
	int		new_ids;

	to_export = NULL;
	new_ids = 0;
	while (*var_ids)
	{
		if (inter_get_var(*envp, var_ids))
		{
			ft_lstadd_back(&to_export, ft_lstnew(*var_ids));
			new_ids++;
		}
		var_ids++;
	}
	if (!to_export)
		return (0);
	new_envp = malloc(sizeof(char *) * (ptrarr_len(*envp) + new_ids));
	ft_memcpy(new_envp, *envp, ctx->envp_len * sizeof(char *));
	add_vars(new_envp, to_export, ctx->envp_len);
	ft_lstclear(&to_export, simple_delete);
	free(*envp);
	*envp = new_envp;
	return (0);
}

int	set_var(char const **vars[], char *new_vars[])
{
	char const *new_vars[];
}

int	unset_vars(char const **envp[], char *var_ids[])
{
	t_list	*to_unset;
	char	**new_envp;
	int		old_ids;
	
	old_ids = 0;
	while (*var_ids)
	{
		if (inter_get_var(*envp, var_ids))
		{
			ft_lstadd_back(&to_unset, ft_lstnew(*var_ids));
			old_ids++;
		}
		mew_envp = malloc(sizeof(char *) * (ptrarr_len(*envp) - old_ids));
		new_envp = remove_vars(envp, new_envp, to_export);
	}
	ft_lstclear(&to_export, simple_delete);
	free(*envp);
	*envp = new_envp;
	return (0);
}

cahr	**remove_vars(char **envp, char new_envp, t_list *vars)
{
	int i = 0;
	int	j = 0;

	while (vars)
	{
		while(envp[i])
		{
			if (ft_strcmp(envp[i], vars->content) == 0)
			{
				i++;
				vars = vars->next;
			}
			else
			{
				new_envp[j] = envp[i];
				j++;
				i++;
			}
		}
	}
	return (new_envp);
}

int	export_cmd(t_scontext *ctx, char *var_ids[])
{
	if (!var_ids)
		env_cmd(ctx);
	else
	{
		export_var(&ctx->envp, var_ids);
		unset_vars(ctx, var_ids);
	}
	return (0);
}
