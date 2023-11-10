/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 20:20:38 by joaoteix          #+#    #+#             */
/*   Updated: 2023/11/10 20:30:27 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <env.h>
#include <unistd.h>

int	cd_val_args(t_shctx *ctx, char **args, char **curpath)
{
	char const	*home = get_var_val(ctx, "HOME");

	if (!*args)
	{
		*curpath = (char *)home;
		if (!home)
		{
			ft_dprintf(STDERR_FILENO, MSH_ERR_PFIX "cd: HOME not set\n");
			return (1);
		}
	}
	else if (*(args + 1) != NULL)
	{
		ft_dprintf(STDERR_FILENO, MSH_ERR_PFIX "cd: too many arguments\n");
		return (1);
	}
	return (0);
}
