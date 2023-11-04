/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 15:18:33 by joaoteix          #+#    #+#             */
/*   Updated: 2023/11/04 19:43:10 by ledos-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include "parser.h"
#include <shell_utils.h>
#include "hdoc.h"
#include <stdio.h>

bool	parse_redirect(t_shctx *ctx, t_list **cursor, t_cmd *cmd, bool *err_flag)
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
		exec_hd(ctx, cmd, get_token(cursor));
	ft_lstadd_back(&cmd->redirs, ft_lstnew(get_token(cursor)));
	get_token(cursor)->type = red_type;
	consume_cursor(cursor);
	return (true);
}

bool	parse_cmd_prefix(t_shctx *ctx, t_list **cursor, t_cmd *cmd, bool *err_flag)
{
	if (!parse_redirect(ctx, cursor, cmd, err_flag))
		return (false);
	while (parse_redirect(ctx, cursor, cmd, err_flag))
		;
	return (true);
}

void	add_cmd_arg(t_list **cursor, t_cmd *cmd)
{
	ft_lstadd_back(&cmd->args, ft_lstnew(&get_token(cursor)->str));
	cmd->arg_n++;
	consume_cursor(cursor);
}

bool	parse_cmd_suffix(t_shctx *ctx, t_list	**cursor, t_cmd *cmd, bool *err_flag)
{
	if (!parse_redirect(ctx, cursor, cmd, err_flag) && !test_cursor(cursor, word))
		return (false);
	if (test_cursor(cursor, word))
		add_cmd_arg(cursor, cmd);
	while (parse_redirect(ctx, cursor, cmd, err_flag) || test_cursor(cursor, word))
		if (test_cursor(cursor, word))
			add_cmd_arg(cursor, cmd);
	return (true);
}

bool	parse_simple_cmd(t_shctx *ctx, t_list **cursor, t_list **pipeline, bool *err_flag)
{
	t_cmd	*cmd;

	cmd = ft_calloc(1, sizeof(t_cmd));
	if (parse_cmd_prefix(ctx, cursor, cmd, err_flag))
	{
		if (test_cursor(cursor, word))
		{
			add_cmd_arg(cursor, cmd);
			parse_cmd_suffix(ctx, cursor, cmd, err_flag);
		}
		ft_lstadd_back(pipeline, ft_lstnew(cmd));
		return (true);
	}
	else if (test_cursor(cursor, word))
	{
		add_cmd_arg(cursor, cmd);
		parse_cmd_suffix(ctx, cursor, cmd, err_flag);
		ft_lstadd_back(pipeline, ft_lstnew(cmd));
		return (true);
	}
	del_cmd(cmd);
	return (false);
}
