/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/05 15:57:50 by joaoteix          #+#    #+#             */
/*   Updated: 2022/11/09 20:38:53 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <stdlib.h>

char	*ft_strtrim(char const *s, const char *set)
{
	const char	*wstart;
	size_t		len;

	if (!s || !set)
		return (NULL);
	len = 0;
	wstart = s;
	while (*s)
	{
		if (!ft_strchr(set, *s))
		{
			if (len == 0)
				wstart = s;
			len = s - wstart + 1;
		}
		s++;
	}
	return (ft_substr(wstart, 0, len));
}
