/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 19:04:45 by joaoteix          #+#    #+#             */
/*   Updated: 2022/11/10 18:09:29 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <stddef.h>

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	lillen;

	if (*little)
	{
		lillen = ft_strlen(little);
		while (len >= lillen && len > 0 && *big != 0)
		{
			if (ft_strncmp(big, little, lillen) == 0)
				return ((char *)big);
			len--;
			big++;
		}
		return (NULL);
	}
	return ((char *)big);
}
