/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 14:22:48 by joaoteix          #+#    #+#             */
/*   Updated: 2023/04/28 14:39:17 by joaoteix         ###   ########.fr       */
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

char const	*inter_get_var(char const *envp[], char const *const var_id)
{
	while (*envp)
	{
		if (strcmp(*envp, var_id) == 0)
			return (*envp);
		envp++;
	}
	return (NULL);
}

char const	*sctx_get_param(t_scontext *ctx, char const *const var_id)
{
	return (inter_get_var(ctx->svars, var_id));
}

char const	*sctx_get_var(t_scontext *ctx, char const *const var_id)
{
	char const	*ret;

	ret = inter_get_var(ctx->envp, var_id);
	if (ret)
		return (ret);
	ret = inter_get_var(ctx->svars, var_id);
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

/*
int	export_var(char const **envp[], char *var_ids[])
{
	t_list	*to_export;
	char	**new_envp;
	int		new_ids;

	to_export = NULL;
	new_ids = 0;
	while (*var_ids)
	{
		if (inter_getenv(*envp,  var_ids))
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
	return (0);
}

int	export_cmd(t_scontext *ctx, char *var_ids[])
{
	export_var(&ctx->envp, var_ids);
	ctx->envp_len += new_ids;
	return (0);
}*/
