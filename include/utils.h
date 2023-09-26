/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 23:37:19 by joaoteix          #+#    #+#             */
/*   Updated: 2023/09/25 21:22:30 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

// Lexer utils
# include <minishell.h>

int		is_blank(char c);
int		is_num(char *str);
int		is_op(char c);
int		is_wordchar(char c);
int		is_blank_str(char str[]);
void	free_ptrarr(void **arr, void (*del)(void *));

// Struct utils
t_token	*get_token(t_list **list);

// Parser utils
void	read_hd(t_cmd *cmd, t_token *delimtok);

// Internal shell context interface
char	*sctx_getenv(t_shctx *ctx, char *var_id);
void	sctx_destroy(t_shctx *ctx);

#endif
