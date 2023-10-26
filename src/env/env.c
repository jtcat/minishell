/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcat <joaoteix@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 18:39:36 by jcat              #+#    #+#             */
/*   Updated: 2023/10/25 10:04:03 by jcat             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"

char	*get_var_id(char const *var)
{
	return (ft_substr(var, 0, ft_strchr(var, '=') - var));
}

t_list	*get_var_ref(t_shctx *ctx, char const *varname)
{
	t_list			*env_i;
	const int		varname_len = ft_strlen(varname);

	env_i = ctx->envp;
	while (env_i && ft_strncmp(env_i->content, varname, varname_len) != 0)
		env_i = env_i->next;
	return (env_i);
}

char	*get_var_val(t_shctx *ctx, char const *varname)
{
	return (ft_strchr(get_var_ref(ctx, varname)->content, '=') + 1);
}

t_list	*dup_envp(t_shctx *ctx, char const *envp[])
{
	t_list	*new_envp;

	new_envp = NULL;
	ctx->envp_len = 0;
	while (envp[ctx->envp_len])
		ft_lstadd_back(&new_envp, ft_lstnew(ft_strdup(envp[ctx->envp_len++])));
	return (new_envp);
}


