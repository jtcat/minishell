/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcat <joaoteix@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 12:01:35 by jcat              #+#    #+#             */
/*   Updated: 2023/10/30 12:06:05 by jcat             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

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

int	val_var_id(char *var)
{
	if (!(ft_isalpha(*var) || *var == '_'))
		return (0);
	while (*var && *var != '=' && (ft_isalnum(*var) || *var == '_'))
		var++;
	return (*var == '\0' || *var == '=');
}
