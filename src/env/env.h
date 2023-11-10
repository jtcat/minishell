/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcat <joaoteix@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 18:40:14 by jcat              #+#    #+#             */
/*   Updated: 2023/11/10 17:51:38 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include <minishell.h>

char	*get_var_id(char const *var);
t_dlist	*get_var_ref(t_shctx *ctx, char const *varname);
char	*get_var_val(t_shctx *ctx, char const *varname);
void	init_envp(t_shctx *ctx, char const *envp[]);
t_dlist	*get_export_ref(t_shctx *ctx, char const *var);
void	set_var(t_shctx *ctx, const char *var, const char *val);
void	unset_var(t_shctx *ctx, char *var);
int		val_var_id(char *var);
#endif
