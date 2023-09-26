/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 15:18:33 by joaoteix          #+#    #+#             */
/*   Updated: 2023/09/26 01:54:41 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "grammar.h"
#include "libft.h"
#include "minishell.h"
#include "utils.h"
#include <stdio.h>

bool	parse_redirect(t_list **cursor, t_cmd *cmd, bool *err_flag)
{
	t_token_type	red_type;

	if (!(test_cursor(cursor, red_in)
			|| test_cursor(cursor, red_out)
			|| test_cursor(cursor, red_out_ap)
			|| test_cursor(cursor, here_doc)))
		return (false);
	red_type = get_token(cursor)->type;
	consume_cursor(cursor);
	if (!test_cursor(cursor, word))
		return (synt_err("redirect_err", cursor, err_flag));
	if (red_type == here_doc)
	 	read_hd(cmd, get_token(cursor));
	else
		ft_lstadd_back(&cmd->redirs, ft_lstnew(get_token(cursor)));
	consume_cursor(cursor);
	return (true);
}

bool	parse_cmd_prefix(t_list **cursor, t_cmd *cmd, bool	*err_flag)
{
	if (!parse_redirect(cursor, cmd, err_flag))
		return (false);
	while (parse_redirect(cursor, cmd, err_flag))
		;
	return (true);
}

void	add_cmd_arg(t_list **cursor, t_cmd *cmd)
{
	ft_lstadd_back(&cmd->args, ft_lstnew(&get_token(cursor)->str));
	cmd->arg_n++;
	consume_cursor(cursor);
}

bool	parse_cmd_suffix(t_list	**cursor, t_cmd *cmd, bool *err_flag)
{
	if (!parse_redirect(cursor, cmd, err_flag) && !test_cursor(cursor, word))
		return (false);
	if (test_cursor(cursor, word))
		add_cmd_arg(cursor, cmd);
	while (parse_redirect(cursor, cmd, err_flag) || test_cursor(cursor, word))
		if (test_cursor(cursor, word))
			add_cmd_arg(cursor, cmd);
	return (true);
}

bool	parse_simple_cmd(t_list **cursor, t_list **pipeline, bool *err_flag)
{
	t_cmd	*cmd;

	cmd = ft_calloc(1, sizeof(t_cmd));
	cmd->hd_fd = -1;
	if (parse_cmd_prefix(cursor, cmd, err_flag))
	{
		if (test_cursor(cursor, word))
		{
			add_cmd_arg(cursor, cmd);
			parse_cmd_suffix(cursor, cmd, err_flag);
		}
		ft_lstadd_back(pipeline, ft_lstnew(cmd));
		return (true);
	}
	else if (test_cursor(cursor, word))
	{
		add_cmd_arg(cursor, cmd);
		parse_cmd_suffix(cursor, cmd, err_flag);
		ft_lstadd_back(pipeline, ft_lstnew(cmd));
		return (true);
	}
	del_cmd(cmd);
	return (false);
}
