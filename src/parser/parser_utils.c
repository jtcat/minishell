/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 15:23:11 by joaoteix          #+#    #+#             */
/*   Updated: 2023/09/26 14:30:29 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "grammar.h"
#include "libft.h"
#include "minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

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
// Revisit hd_fd dup, close and set. It's left open at the end
void	read_hd(t_cmd *cmd, t_token *delimtok)
{
	const char	*delim = delimtok->str;
	char		*line;
	int			pipefd[2];

	if (cmd->hd_fd > -1)
		close(cmd->hd_fd);
	pipe(pipefd);
	line = readline(HD_PROMPT);
	while (line)
	{
		if (ft_strcmp(line, delim) == 0 && line[ft_strlen(delim)] == '\0')
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, pipefd[1]);
		ft_putchar_fd('\n', pipefd[1]);
		free(line);
		line = readline(HD_PROMPT);
	}
	close(pipefd[1]);
	cmd->hd_fd = pipefd[0];
	ft_lstadd_back(&cmd->redirs, ft_lstnew(delimtok));
}
