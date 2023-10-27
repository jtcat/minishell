/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcat <joaoteix@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 18:39:36 by jcat              #+#    #+#             */
/*   Updated: 2023/10/27 02:10:34 by jcat             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"

char	*get_var_id(char const *var)
{
	return (ft_substr(var, 0, ft_strchr(var, '=') - var));
}

t_dlist	*get_var_ref(t_shctx *ctx, char const *var)
{
	t_dlist			*env_i;
	const int		varname_len = ft_strchr(var, '=') - var;

	env_i = ctx->exports;
	while (env_i && ft_strncmp(env_i->content, var, varname_len) != 0)
		env_i = env_i->next;
	return (env_i);
}

char	*get_var_val(t_shctx *ctx, char const *varname)
{
	return (ft_strchr(get_var_ref(ctx, varname)->content, '=') + 1);
}

t_dlist	*dup_envp(t_shctx *ctx, char const *envp[])
{
	t_dlist	*new_envp;

	new_envp = NULL;
	ctx->envp_len = 0;
	while (envp[ctx->envp_len])
		ft_dlstadd_back(&new_envp, ft_dlstnew(ft_strdup(envp[ctx->envp_len++])));
	return (new_envp);
}


