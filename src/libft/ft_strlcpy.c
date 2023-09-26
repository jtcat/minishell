/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 15:24:52 by joaoteix          #+#    #+#             */
/*   Updated: 2022/11/06 18:10:01 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <stddef.h>

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	src_l;
	size_t	i;

	src_l = ft_strlen(src);
	if (size < 1)
		return (src_l);
	i = 0;
	while (src[i] && i < (size - 1))
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (src_l);
}
