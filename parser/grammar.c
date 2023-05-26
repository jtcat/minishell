/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 15:16:27 by joaoteix          #+#    #+#             */
/*   Updated: 2023/05/26 15:20:17 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "grammar.h"

bool	parse_pipeline_suffix(t_list **curs, t_ppline **pipeline,
		bool *err_flag)
{
	if (!test_cursor(curs, pipe_op))
		return (false);
	consume_cursor(curs);
	if (!parse_simple_cmd(curs, &(*pipeline)->cmds, err_flag))
		return (synt_err("pipeline_suffix_err", curs, err_flag));
	(*pipeline)->pipe_n++;
	return (true);
}

bool	parse_pipeline(t_list **cursor, t_list **pipe_list, bool	*err_flag)
{
	t_ppline	*pipeline;

	pipeline = ft_calloc(1, sizeof(t_ppline));
	pipeline->op = lst_no_op;
	if (!parse_simple_cmd(cursor, &pipeline->cmds, err_flag))
		return (false);
	while (parse_pipeline_suffix(cursor, &pipeline, err_flag))
		;
	ft_lstadd_front(pipe_list, ft_lstnew(pipeline));
	return (true);
}

bool	parse_list_suffix(t_list **cursor, t_list **pipe_list, bool	*err_flag)
{
	if (!test_cursor(cursor, lst_and) && !test_cursor(cursor, lst_or))
		return (false);
	((t_ppline *)(*pipe_list)->content)->op = get_token(cursor)->type;
	consume_cursor(cursor);
	if (!parse_pipeline(cursor, pipe_list, err_flag))
		return (synt_err("list_suffix_err", cursor, err_flag));
	return (true);
}

bool	parse_list(t_list **cursor, t_list **pipe_list, bool *err_flag)
{
	if (!parse_pipeline(cursor, pipe_list, err_flag))
		return (false);
	while (parse_list_suffix(cursor, pipe_list, err_flag))
		;
	return (true);
}

bool	parse_input(t_list *input, t_list **pipe_list)
{
	bool	err_flag;

	err_flag = false;
	parse_list(&input, pipe_list, &err_flag);
	if (!test_cursor(&input, eof))
		return (synt_err("parse_tokens", &input, &err_flag));
	return (!err_flag);
}
