/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 15:23:11 by joaoteix          #+#    #+#             */
/*   Updated: 2023/09/07 21:41:13 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "grammar.h"
#include "libft.h"

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
	if (token->type == eof)
		ft_putstr_fd(MSH_ERR_PFIX "unexpected end of input\n", STDERR_FILENO);
	else
		ft_dprintf(STDERR_FILENO,
			MSH_ERR_PFIX "error near unexpected token `%s'\n", token->str);
	*parser_err_flag = true;
	return (false);
}

void	assign_redirect(t_cmd *cmd, t_token_type red_type, char *filename)
{
	if (red_type == red_in)
		cmd->red_in = filename;
	else if (red_type == red_out)
	{
		cmd->red_out = filename;
		cmd->ap_out = false;
	}
	else if (red_type == red_out_ap)
	{
		cmd->red_out = filename;
		cmd->ap_out = true;
	}
	else if (red_type == here_doc)
	{
		ft_lstadd_back(&cmd->hd_delims, ft_lstnew(filename));
	}
}
