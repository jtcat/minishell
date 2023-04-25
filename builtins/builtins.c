/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 14:22:48 by joaoteix          #+#    #+#             */
/*   Updated: 2023/04/26 00:01:19 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Minishell.h>

int	echo(char str[], int opt_n)
{	
	while(*str)
	{
		if (opt_n && *str == '\n')
			break;
		ft_putstr_fd((char *)str, STDOUT_FILENO);
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

int	pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	printf("%s\n",cwd);
	free(cwd);
	return (0);
}

int	env(char *env[])
{
	while(*env)
		printf("%s\n",*(env++));
	return(0);
}
