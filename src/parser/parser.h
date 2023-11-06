/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcat <joaoteix@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 19:26:27 by jcat              #+#    #+#             */
/*   Updated: 2023/11/06 17:05:09 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <minishell.h>
# include <unistd.h>
# include <stdbool.h>

// Parser entry point
bool	parse_input(t_shctx *ctx, t_list *input, t_list **pipe_list);

// Parser utils
t_token	*get_token(t_list **cursor);
bool	test_cursor(t_list **cursor, t_token_type type);
void	consume_cursor(t_list **cursor);
bool	synt_err(char errctx[], t_list **cursor, bool	*parser_err_flag);
void	assign_redirect(t_cmd *cmd, t_token_type red_type, char *filename);
bool	parse_simple_cmd(t_shctx *ctx, t_list **cursor, t_list **pipeline, bool *err_flag);
t_token	*get_token(t_list **list);
#endif
