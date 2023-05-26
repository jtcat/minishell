/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 15:20:32 by joaoteix          #+#    #+#             */
/*   Updated: 2023/05/26 15:34:03 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GRAMMAR_H
# define GRAMMAR_H

# include <minishell.h>
# include <unistd.h>
# include <stdbool.h>

t_token	*get_token(t_list **cursor);
bool	test_cursor(t_list **cursor, t_token_type type);
void	consume_cursor(t_list **cursor);
bool	synt_err(char errctx[], t_list **cursor, bool	*parser_err_flag);
void	assign_redirect(t_cmd *cmd, t_token_type red_type, char *filename);
bool	parse_simple_cmd(t_list **cursor, t_list **pipeline, bool *err_flag);

#endif
