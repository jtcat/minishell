/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 15:15:56 by joaoteix          #+#    #+#             */
/*   Updated: 2023/05/12 01:56:00 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <minishell.h>
#include <readline/chardefs.h>
#include <stdbool.h>
#include <unistd.h>

/*
 * Command grammar:
 * simple_cmd:		assigment 
 * 					| redirect
 * assignment:		ID '=' WORD
 * redirect:		('<' | '<<' | '>' | '>>') WORD
 * pipeline:		simple_cmd | pipeline_suffix*
 * pipeline_suffix:	'|' simple_cmd
 * list:			pipeline (('&&' | '||') pipeline)*
 * cmd:				list+ '\n'
 */

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

bool	synt_err(t_list **cursor, bool	*parser_err_flag)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
	ft_putstr_fd(get_token(cursor)->str, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	*parser_err_flag = true;
	return (false);
}

const char	*lex_name(t_list **input)
{
	if (ft_isdigit(*(input++)))
		return (NULL);
	if (!ft_isalnum(*input) && !(*input == '_'))
		return (NULL);
	input++;
	while (ft_isalnum(*input) || (*input == '_'))
		input++;
	return (input);
}

const char	*lex_assignment(t_list **input)
{
	const char	*name;

	name = lex_name(input);
	if (!name)
		return(NULL);
	if (*(name++) != '=')
		return(NULL);
	input = lex_word(input);
	return (input);
}

void	assign_redirect(t_cmd *cmd, t_token_type red_type, char const *filename)
{
	if (red_type == red_in)
		cmd->red_in = filename;
	else if (red_type == red_out)
	{
		cmd->red_out = filename;
		cmd->append_out = false;
	}
	else if (red_type == red_out_ap)
	{
		cmd->red_out = filename;
		cmd->append_out = true;
	}
	else if (red_type == here_doc)
	{
		cmd->hd_delim = filename;
	}
}

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
		return (synt_err(cursor, err_flag));
	assign_redirect(red_type, get_token(cursor)->str);
	consume_cursor(cursor);
	return (true);
}

bool	parse_cmd_prefix(t_list **cursor, t_cmd *cmd, bool	*err_flag)
{
	if (!parse_redirect(cursor, cmd, err_flag) && !lex_assignment(cursor))
		return (false);
	while (parse_redirect(cursor, cmd, err_flag) || lex_assignment(cursor))
		;
	return (true);
}

bool	parse_cmd_suffix(t_list	**cursor, t_cmd *cmd, bool *err_flag)
{
	if (!parse_redirect(cursor, cmd, err_flag) && !test_cursor(cursor, word))
		return (false);
	while (parse_redirect(cursor, cmd, err_flag) || test_cursor(cursor, word))
		;
	return (true);
}

bool	parse_simple_cmd(t_list **cursor, t_list *pipeline, bool *err_flag)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (parse_cmd_prefix(cursor, cmd, err_flag))
	{
		if (test_cursor(cursor, word))
		{
			ft_lstadd_back(&cmd->args, ft_lstnew(get_token(cursor)->str));
			consume_cursor(cursor);
			parse_cmd_suffix(cursor, cmd, err_flag);
			return (true);
		}
		ft_lstadd_back(&pipeline, ft_lstnew(cmd));
		return (true);
	}
	else if (test_cursor(cursor, word))
	{
		ft_lstadd_back(&cmd->args, ft_lstnew(get_token(cursor)->str));
		consume_cursor(cursor);
		parse_cmd_suffix(cursor, cmd, err_flag);
		ft_lstadd_back(&pipeline, ft_lstnew(cmd));
		return (true);
	}
	destroy_cmd(cmd);
	return (false);
}

bool	parse_pipeline_suffix(t_list **cursor, t_list *pipeline, bool *err_flag)
{
	if (!test_cursor(cursor, pipe_op))
		return (false);
	consume_cursor(cursor);
	if (!parse_simple_cmd(cursor, pipeline, err_flag))
		return (synt_err(cursor, err_flag));
	return (true);
}

bool	parse_pipeline(t_list **cursor, t_list *pipe_list, bool	*err_flag)
{
	t_list	*pipeline;

	pipeline = NULL;
	if (!parse_simple_cmd(cursor, pipeline, err_flag))
		return (false);
	consume_cursor(cursor);
	while (parse_pipeline_suffix(cursor, pipeline, err_flag))
		;
	ft_lstadd_front(&pipe_list, ft_lstnew(pipeline));
	return (true);
}

bool	parse_list_suffix(t_list **cursor, t_list *pipe_list, bool	*err_flag)
{
	if (!test_cursor(cursor, lst_and) || !test_cursor(cursor, lst_or))
		return (false);
	consume_cursor(cursor);
	if (!parse_pipeline(cursor, pipe_list, err_flag))
		return (synt_err(cursor, err_flag));
	return (true);
}

bool	parse_list(t_list **cursor, t_list *pipe_list)
{
	bool	err_flag;

	err_flag = false;
	if (!parse_pipeline(cursor, pipe_list, &err_flag))
		return (synt_err(cursor, &err_flag));
	while (parse_list_suffix(cursor, pipe_list, &err_flag))
	{
	}
	return (err_flag);
}
