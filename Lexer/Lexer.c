/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leborges <leborges@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 17:29:48 by leborges          #+#    #+#             */
/*   Updated: 2023/04/25 14:05:27 by leborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

char	*parse_quotes(char *str, t_list *token_head)

t_list	*split_tokens(char *str)
{
	t_list *token_head;
	int		wordlen;

	token_head = NULL;
	wordlen = 0;
	while (1)
	{
		//if (*s == '"')
		//	s = parse_quotes(s, token_head);
		/*else*/ if (*str != ' ' && *str != '\0')
			wordlen++;
		else if (wordlen > 0)
		{
			ft_lstadd_back(&token_head, ft_lstnew(ft_substr(str - wordlen, 0, wordlen)));
			wordlen = 0;
		}
		if (*str == '\0')
			break ;
		str++;
	}
	return (token_head);
}
