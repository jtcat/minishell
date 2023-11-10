/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 23:37:19 by joaoteix          #+#    #+#             */
/*   Updated: 2023/11/10 14:58:51 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_UTILS_H
# define SHELL_UTILS_H

int		is_blank(char c);
int		is_op(char c);
int		is_wordchar(char c);
int		is_blank_str(char str[]);
char	*replace_str(char **dest_ref, char *newstr);
#endif
