/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leborges <leborges@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 17:29:48 by leborges          #+#    #+#             */
/*   Updated: 2023/04/24 20:04:00 by leborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Minishell.h"


t_list	*split_tokens(char *str)
{
	t_list *token_head;
	int		wordlen;

	token_head = NULL;
	wordlen = 0;
	while (1)
	{
		if (*s != c && *s != '\0')
			wordlen++;
		else if (wordlen > 0)
		{
			ft_lstadd_back(&token_head, ft_substr(s - wordlen, 0, wordlen));
			wordlen = 0;
		}
		if (*s == '\0')
			break ;
		s++;
	}
	return (token_head);
}