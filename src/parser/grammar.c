/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 15:16:27 by joaoteix          #+#    #+#             */
/*   Updated: 2023/11/07 16:42:35 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include "parser.h"

bool	parse_pipeline_suffix(t_shctx *ctx, t_list **curs, t_ppline **pipeline,
		bool *err_flag)
{
	if (!test_cursor(curs, pipe_op))
		return (false);
	consume_cursor(curs);
	if (!parse_simple_cmd(ctx, curs, &(*pipeline)->cmds, err_flag))
		return (synt_err("pipeline_suffix_err", curs, err_flag));
	return (true);
}

bool	parse_pipeline(t_shctx *ctx, t_list **cursor,
		t_list **pipe_list, bool *err_flag)
{
	t_ppline	*pipeline;

	pipeline = ft_calloc(1, sizeof(t_ppline));
	pipeline->op = lst_no_op;
	if (!parse_simple_cmd(ctx, cursor, &pipeline->cmds, err_flag))
		return (false);
	while (parse_pipeline_suffix(ctx, cursor, &pipeline, err_flag))
		;
	ft_lstadd_back(pipe_list, ft_lstnew(pipeline));
	return (true);
}

bool	parse_list_suffix(t_shctx *ctx, t_list **cursor, \
		t_list **pipe_list, bool	*err_flag)
{
	t_token_type	op;

	if (!test_cursor(cursor, lst_and) && !test_cursor(cursor, lst_or))
		return (false);
	op = get_token(cursor)->type;
	consume_cursor(cursor);
	if (!parse_pipeline(ctx, cursor, pipe_list, err_flag))
		return (synt_err("list_suffix_err", cursor, err_flag));
	((t_ppline *)(ft_lstlast(*pipe_list))->content)->op = op;
	return (true);
}

bool	parse_list(t_shctx *ctx, t_list **cursor, \
		t_list **pipe_list, bool *err_flag)
{
	if (!parse_pipeline(ctx, cursor, pipe_list, err_flag))
		return (false);
	while (parse_list_suffix(ctx, cursor, pipe_list, err_flag))
		;
	return (true);
}

bool	parse_input(t_shctx *ctx, t_list *input, t_list **pipe_list)
{
	bool	err_flag;

	err_flag = false;
	parse_list(ctx, &input, pipe_list, &err_flag);
	if (!test_cursor(&input, newline))
		return (synt_err("parse_tokens", &input, &err_flag));
	return (!err_flag);
}
