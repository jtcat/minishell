/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 14:22:48 by joaoteix          #+#    #+#             */
/*   Updated: 2023/04/26 02:24:07 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <minishell.h>
#include <unistd.h>

// Builtin utilities
//
// Return values represent exit status

int	echo(char str[], int opt_n)
{	
	ft_putstr_fd((char *)str, STDOUT_FILENO);
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

int	pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}

int	env(t_scontext *ctx)
{
	char *const *iter = ctx->envp;

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
		vars =  vars->next;
	}
}

char const	*sctx_get_var(t_scontext *ctx, char const *const var_id)
{
	int	i;

	i = 0;
	while (ctx->envp[i])
	{
		if (strcmp(ctx->envp[i], var_id) == 0)
			return (ctx->envp[i]);
		i++;
	}
	i = 0;
	while (ctx->svars[i])
	{
		if (strcmp(ctx->envp[i], var_id) == 0)
			return (ctx->envp[i]);
		i++;
	}
	return (NULL);
}

int	export(t_scontext *ctx, char *var_ids[])
{
	t_list	*to_export;
	char	**new_envp;
	int		new_ids;

	to_export = NULL;
	new_ids = 0;
	while (*var_ids)
	{
		if (sctx_get_var(ctx,  *var_ids))
		{
			ft_lstadd_back(&to_export, ft_lstnew(*var_ids));
			new_ids++;
		}
		var_ids++;
	}
	if (!to_export)
		return (0);
	new_envp = malloc(sizeof(char *) * (ctx->envp_len + new_ids));
	ft_memcpy(new_envp, ctx->envp, ctx->envp_len * sizeof(char *));
	add_vars(new_envp, to_export, ctx->envp_len);
	ft_lstclear(&to_export, simple_delete);
	free(ctx->envp);
	ctx->envp = new_envp;
	ctx->envp_len += new_ids;
	return (0);
}
