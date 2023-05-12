/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leborges <leborges@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 17:29:48 by leborges          #+#    #+#             */
/*   Updated: 2023/05/12 19:07:53 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <str_utils.h>

t_token	*create_token(char *s, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	token->str = s;
	token->type = type;
	return (token);
}

void	store_token(t_list **token_head, char **str, int *wordlen,
		t_token_type *type)
{
	if (*wordlen == 0)
		return ;
	ft_lstadd_back(token_head,
		ft_lstnew(
			create_token(ft_substr(*str - *wordlen, 0, *wordlen), *type)));
	*wordlen = 0;
	*type = word;
}

void	parse_quotes(char **str, int *wordlen, t_token_type *type)
{
	const char	*end_quote;

	while (is_quote(**str))
	{
		end_quote = ft_strchr(*str + 1, **str);
		if (end_quote)
		{
			*wordlen += ++end_quote - *str;
			*str = (char *)end_quote;
			*type = word;
		}
		else
		{
			(*str)++;
			*wordlen = 0;
		}
	}
}

void	parse_operators(t_list **token_head, char **str,
		int *wordlen, t_token_type *type)
{
	if (**str == '>' || **str == '<')
	{
		if (*((*str) + 1) == **str)
		{
			*wordlen += 2;
			*type = (**str == '>') * red_out_ap + (**str == '<') * here_doc;
			*str += 2;
		}
		else
		{
			*wordlen += 1;
			*type = (**str == '>') * red_out + (**str == '<') * red_in;
			*str += 1;
		}
	}
	else if (**str == '|')
	{
		*wordlen += 1;
		*str += 1;
		*type = pipe_op;
	}
	store_token(token_head, str, wordlen, type);
}

void	parse_wordchar(char **str, int *wordlen)
{
	(*str)++;
	(*wordlen)++;
}

int	parse_delim(t_list **token_head, char **str,
		int *wordlen, t_token_type *type)
{
	store_token(token_head, str, wordlen, type);
	if (is_op(**str))
		parse_operators(token_head, str, wordlen, type);
	else if (**str == '\0')
		return (1);
	else
		(*str)++;
	return (0);
}

t_list	*split_tokens(char *str)
{
	t_list			*token_head;
	t_token_type	type;
	int				wordlen;

	type = word;
	token_head = NULL;
	wordlen = 0;
	while (1)
	{
		if (is_quote(*str))
			parse_quotes(&str, &wordlen, &type);
		if (is_delim(*str))
		{
			if (parse_delim(&token_head, &str, &wordlen, &type))
				break ;
		}
		else
			parse_wordchar(&str, &wordlen);
	}
	return (token_head);
}
