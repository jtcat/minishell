/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 14:22:48 by joaoteix          #+#    #+#             */
/*   Updated: 2023/04/25 20:28:11 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Minishell.h>

int	echo(const char *str, int opt_n)
{	
	while(*str)
	{
		if (opt_n && *str == '\n')
			break;
		ft_putstr_fd(STDOUT_FILENO, *str);
	}
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

int	pwd(t_scontext *ctx)
{
	ft_putstr_fd(STDOUT_FILENO, getenv("PWD"));
	return (0);
}

int	env(const char **env)
{
	while(env)
		ft_putstr_fd(1, *(env++));
}
