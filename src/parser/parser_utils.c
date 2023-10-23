/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 15:23:11 by joaoteix          #+#    #+#             */
/*   Updated: 2023/10/18 18:37:45 by jcat             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "grammar.h"
#include "libft.h"
#include "minishell.h"
#include <stdio.h>
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

// Returns read fd to buffer containing here_doc input
// Should be closed when used
// If delim is unquoted, 
void	read_hd(t_cmd *cmd, t_token *delimtok)
{
	char const	*delim = delimtok->str;
	char		*line;
	int			pipefd[2];

	if (cmd->hd_input != NULL)
		ft_lstclear(&cmd->hd_input, free);
	pipe(pipefd);
	line = readline(HD_PROMPT);
	while (line)
	{
		if (ft_strcmp(line, delim) == 0 && line[ft_strlen(delim)] == '\0')
		{
			free(line);
			break ;
		}
		ft_lstadd_back(&cmd->hd_input, ft_lstnew(line));
		line = readline(HD_PROMPT);
	}
	ft_lstadd_back(&cmd->redirs, ft_lstnew(delimtok));
}
