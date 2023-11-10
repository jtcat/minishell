/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcat <joaoteix@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 12:01:35 by jcat              #+#    #+#             */
/*   Updated: 2023/11/10 17:51:55 by joaoteix         ###   ########.fr       */
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
	{
		ft_dlstadd_back(&ctx->envp, ft_dlstnew(newvar));
		ctx->envp_len++;
	}
	free(varname);
}

void	unset_var(t_shctx *ctx, char *var)
{
	t_dlist	*var_ref;

	var_ref = get_var_ref(ctx, var);
	if (var_ref)
	{
		ft_dlstrmone(&ctx->envp, var_ref, free);
		ctx->envp_len--;
	}
	var_ref = get_export_ref(ctx, var);
	if (var_ref)
	{
		ft_dlstrmone(&ctx->exports, var_ref, free);
		ctx->exports_len--;
	}
}

int	val_var_id(char *var)
{
	if (!(ft_isalpha(*var) || *var == '_'))
		return (0);
	while (*var && *var != '=' && (ft_isalnum(*var) || *var == '_'))
		var++;
	return (*var == '\0' || *var == '=');
}
