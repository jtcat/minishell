/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 15:15:56 by joaoteix          #+#    #+#             */
/*   Updated: 2023/05/13 01:37:21 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <minishell.h>
#include <readline/chardefs.h>
#include <stdbool.h>
#include <stdio.h>
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

char	*get_tokentype_str(t_token_type type)
{
	if (type == red_out)
		return "red_out";
	if (type == red_in)
		return "red_in";
	if (type == red_out_ap)
		return "red_out_ap";
	if (type == word)
		return "word";
	if (type == here_doc)
		return "here_doc";
	if (type == pipe_op)
		return "pipe";
	if (type == lst_and)
		return "lst_and";
	if (type == lst_or)
		return "lst_or";
	return "unkown";
}

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

	if (*parser_err_flag)
		return (false);
	printf("%s\n", errctx);
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
		return (synt_err("redirect_err", cursor, err_flag));
	assign_redirect(cmd, red_type, get_token(cursor)->str);
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

bool	parse_cmd_suffix(t_list	**cursor, t_cmd *cmd, bool *err_flag)
{
	if (!parse_redirect(cursor, cmd, err_flag) && !test_cursor(cursor, word))
		return (false);
	if (test_cursor(cursor, word))
		consume_cursor(cursor);
	while (parse_redirect(cursor, cmd, err_flag) || test_cursor(cursor, word))
		if (test_cursor(cursor, word))
			consume_cursor(cursor);
	return (true);
}

void	destroy_cmd(t_cmd *cmd)
{
	ft_lstclear(&cmd->args, free);
	if (cmd->red_in)
		free(cmd->red_in);
	if (cmd->red_out)
		free(cmd->red_out);
	if (cmd->hd_delim)
		free(cmd->hd_delim);
	free(cmd);
}

bool	parse_simple_cmd(t_list **cursor, t_list **pipeline, bool *err_flag)
{
	t_cmd	*cmd;

	cmd = ft_calloc(1, sizeof(t_cmd));
	if (parse_cmd_prefix(cursor, cmd, err_flag))
	{
		if (test_cursor(cursor, word))
		{
			ft_lstadd_back(&cmd->args, ft_lstnew(get_token(cursor)->str));
			consume_cursor(cursor);
			parse_cmd_suffix(cursor, cmd, err_flag);
		}
		ft_lstadd_back(pipeline, ft_lstnew(cmd));
		return (true);
	}
	else if (test_cursor(cursor, word))
	{
		ft_lstadd_back(&cmd->args, ft_lstnew(get_token(cursor)->str));
		consume_cursor(cursor);
		parse_cmd_suffix(cursor, cmd, err_flag);
		ft_lstadd_back(pipeline, ft_lstnew(cmd));
		return (true);
	}
	destroy_cmd(cmd);
	return (false);
}

bool	parse_pipeline_suffix(t_list **curs, t_list **pipeline, bool *err_flag)
{
	if (!test_cursor(curs, pipe_op))
		return (false);
	consume_cursor(curs);
	if (!parse_simple_cmd(curs, pipeline, err_flag))
		return (synt_err("pipeline_suffix_err", curs, err_flag));
	return (true);
}

bool	parse_pipeline(t_list **cursor, t_list **pipe_list, bool	*err_flag)
{
	t_list	*pipeline;

	pipeline = NULL;
	if (!parse_simple_cmd(cursor, &pipeline, err_flag))
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
