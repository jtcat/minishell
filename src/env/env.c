/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcat <joaoteix@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 18:39:36 by jcat              #+#    #+#             */
/*   Updated: 2023/11/07 16:57:21 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"

char	*get_var_id(char const *var)
{
	size_t	len;

	len = 0;
	while (ft_isalnum(var[len]) || var[len] == '_')
		len++;
	return (ft_substr(var, 0, len));
}

t_dlist	*get_var_ref(t_shctx *ctx, char const *var)
{
	t_dlist	*env_i;
	char	*varname;
	size_t	namelen;

	varname = get_var_id(var);
	namelen = ft_strlen(varname);
	env_i = ctx->envp;
	while (env_i && ft_strncmp(env_i->content, varname, namelen) != 0)
		env_i = env_i->next;
	free(varname);
	return (env_i);
}

t_dlist	*get_export_ref(t_shctx *ctx, char const *var)
{
	t_dlist	*exp_i;
	char	*varname;
	size_t	namelen;

	varname = get_var_id(var);
	namelen = ft_strlen(varname);
	exp_i = ctx->exports;
	while (exp_i && ft_strncmp(exp_i->content, varname, namelen) != 0)
		exp_i = exp_i->next;
	free(varname);
	return (exp_i);
}

char	*get_var_val(t_shctx *ctx, char const *varname)
{
	t_dlist const	*var_ref = get_var_ref(ctx, varname);
	char			*var_val;

	if (!var_ref)
		return (NULL);
	var_val = ft_strchr(var_ref->content, '=');
	if (!var_val)
		return (NULL);
	return (var_val + 1);
}

void	init_envp(t_shctx *ctx, char const *envp[])
{
	ctx->envp = NULL;
	ctx->exports = NULL;
	ctx->envp_len = 0;
	while (envp[ctx->envp_len])
	{
		ft_dlstadd_back(&ctx->envp,
			ft_dlstnew(ft_strdup(envp[ctx->envp_len])));
		ft_dlstadd_back(&ctx->exports,
			ft_dlstnew(get_var_id(envp[ctx->envp_len++])));
	}
	ctx->exports_len = ctx->envp_len;
}
