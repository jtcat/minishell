/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 15:23:11 by joaoteix          #+#    #+#             */
/*   Updated: 2023/11/01 17:31:08 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

t_token	*get_token(t_list **cursor)
{
	return ((t_token *)((*cursor)->content));
}

bool	test_cursor(t_list **cursor, t_token_type type)
{
	return (get_token(cursor)->type == type);
}

void	consume_cursor(t_list **cursor)
{
	*cursor = (*cursor)->next;
}

bool	synt_err(char errctx[], t_list **cursor, bool	*parser_err_flag)
{
	t_token *const	token = get_token(cursor);

	(void)errctx;
	if (*parser_err_flag)
		return (false);
	ft_dprintf(STDERR_FILENO,
		MSH_ERR_PFIX "error near unexpected token `%s'\n", token->str);
	*parser_err_flag = true;
	return (false);
}
