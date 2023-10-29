/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcat <joaoteix@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 18:40:14 by jcat              #+#    #+#             */
/*   Updated: 2023/10/26 17:55:14 by jcat             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

#include <minishell.h>

char	*get_var_id(char const *var);
t_dlist	*get_var_ref(t_shctx *ctx, char const *varname);
char	*get_var_val(t_shctx *ctx, char const *varname);
t_dlist	*dup_envp(t_shctx *ctx, char const *envp[]);

#endif