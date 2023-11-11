/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leborges <leborges@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 17:29:48 by leborges          #+#    #+#             */
/*   Updated: 2023/11/11 01:16:13 by jcat             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	store_token(t_list **token_list, char *start, char *end,
		t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (type == newline)
		token->str = ft_strdup("newline");
	else
		token->str = ft_substr(start, 0, end - start + 1);
	token->type = type;
	ft_lstadd_back(token_list, ft_lstnew(token));
}

bool	lex_word(t_list **token_list, char **cursor)
{
	char	*word_start;

	word_start = *cursor;
	while (is_wordchar(**cursor))
		(*cursor)++;
	store_token(token_list, word_start, *cursor - 1, word);
	return (true);
}

bool	lex_op(t_list **token_list, char **cursor)
{
	char			*token_start;
	t_token_type	type;

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

t_list	*split_tokens(char *str)
{
	t_list			*token_list;

	token_list = NULL;
	while (*str)
	{
		if (is_op(*str))
			lex_op(&token_list, &str);
		else if (is_wordchar(*str))
			lex_word(&token_list, &str);
		else
		 	str++;
	}
	store_token(&token_list, NULL, NULL, newline);
	return (token_list);
}
