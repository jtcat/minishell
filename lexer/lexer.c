/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leborges <leborges@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 17:29:48 by leborges          #+#    #+#             */
/*   Updated: 2023/05/26 15:12:12 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "utils.h"
#include <unistd.h>
#include <minishell.h>

void	store_token(t_list **token_list, char *start, char *end,
		t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (type == eof)
		token->str = ft_strdup("");
	else
		token->str = ft_substr(start, 0, end - start + 1);
	token->type = type;
	ft_lstadd_back(token_list, ft_lstnew(token));
}

bool	lex_word(t_list **token_list, char **cursor)
{
	char	*word_start;
	char	*quote_end;

	if (!is_wordchar(**cursor))
		return (false);
	word_start = *cursor;
	while (is_wordchar(**cursor))
	{
		if (**cursor == '\'' || **cursor == '"')
		{
			quote_end = ft_strchr(*cursor + 1, **cursor);
			if (quote_end)
				*cursor = quote_end;
			else
			{
				ft_dprintf(STDERR_FILENO, MSH_ERR_PFIX
					"unclosed quote at: `%c'\n", **cursor);
				return (false);
			}
		}
		(*cursor)++;
	}
	store_token(token_list, word_start, *cursor - 1, word);
	return (true);
}

bool	lex_op(t_list **token_list, char **cursor)
{
	char			*token_start;
	t_token_type	type;

	if (!is_op(**cursor))
		return (false);
	type = word;
	token_start = (*cursor);
	type = (**cursor == '>') * red_out
		+ (**cursor == '<') * red_in
		+ (**cursor == '|') * pipe_op;
	(*cursor)++;
	if (**cursor == *token_start)
	{
		type = (**cursor == '>') * red_out_ap
			+ (**cursor == '<') * here_doc
			+ (**cursor == '&') * lst_and
			+ (**cursor == '|') * lst_or;
		(*cursor)++;
	}
	store_token(token_list, token_start, *cursor - 1, type);
	return (true);
}

void	skip(char	**cursor)
{
	while (**cursor && !is_wordchar(**cursor) && !is_op(**cursor))
		(*cursor)++;
}

t_list	*split_tokens(char *str)
{
	t_list			*token_list;

	token_list = NULL;
	while (lex_op(&token_list, &str) || lex_word(&token_list, &str))
		skip(&str);
	store_token(&token_list, NULL, NULL, eof);
	return (token_list);
}
