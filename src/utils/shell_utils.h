/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 23:37:19 by joaoteix          #+#    #+#             */
/*   Updated: 2023/10/26 18:14:55 by jcat             ###   ########.fr       */
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
#endif
