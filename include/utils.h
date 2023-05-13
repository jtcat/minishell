/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 23:37:19 by joaoteix          #+#    #+#             */
/*   Updated: 2023/05/12 22:32:58 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STR_UTILS_H
# define STR_UTILS_H

int	is_blank(char c);
int	is_quote(char c);
int	is_op(char c);
int	is_wordchar(char c);
int	is_blank_str(char str[]);
#endif
