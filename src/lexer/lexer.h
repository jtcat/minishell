/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcat <joaoteix@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 19:28:27 by jcat              #+#    #+#             */
/*   Updated: 2023/10/24 20:02:23 by jcat             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

#include <minishell.h>
#include <shell_utils.h>
#include <stdio.h>
#include <unistd.h>

// Lexer entry point
t_list	*split_tokens(char *str);

#endif
