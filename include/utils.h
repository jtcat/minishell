/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 23:37:19 by joaoteix          #+#    #+#             */
/*   Updated: 2023/05/19 13:24:18 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STR_UTILS_H
# define STR_UTILS_H

// Lexer utils
#include <minishell.h>
int		is_blank(char c);
int		is_quote(char c);
int		is_op(char c);
int		is_wordchar(char c);
int		is_blank_str(char str[]);
void	free_ptrarr(void **arr, void (*del)(void *));

// Struct utils
t_token *get_token(t_list **list);

#endif
