/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 15:37:23 by joaoteix          #+#    #+#             */
/*   Updated: 2022/11/25 18:35:36 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	destlen;
	size_t	i;

	destlen = 0;
	i = 0;
	while (destlen < size && dest[destlen])
		destlen++;
	while ((i + 1 < (size - destlen)) && src[i])
	{
		dest[destlen + i] = src[i];
		i++;
	}
	if (i > 0)
		dest[destlen + i] = '\0';
	return (ft_strlen(src) + destlen);
}
