/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/07 13:53:20 by joaoteix          #+#    #+#             */
/*   Updated: 2022/11/18 20:22:06 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static char	**ft_genarr(const char *s, char c)
{
	int		word_count;
	int		reading_word;

	if (!s)
		return (NULL);
	word_count = 0;
	reading_word = 0;
	while (1)
	{
		if (*s != c && *s != '\0')
			reading_word = 1;
		else if (reading_word)
		{
			word_count++;
			reading_word = 0;
		}
		if (*s == '\0')
			break ;
		s++;
	}
	return (malloc((word_count + 1) * sizeof(char *)));
}

char	**ft_split(const char *s, char c)
{
	char	**arr;
	int		wordlen;
	int		wordi;

	wordi = 0;
	wordlen = 0;
	arr = ft_genarr(s, c);
	if (!arr)
		return (NULL);
	while (1)
	{
		if (*s != c && *s != '\0')
			wordlen++;
		else if (wordlen > 0)
		{
			arr[wordi++] = ft_substr(s - wordlen, 0, wordlen);
			wordlen = 0;
		}
		if (*s == '\0')
			break ;
		s++;
	}
	arr[wordi] = NULL;
	return (arr);
}
