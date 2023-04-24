/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/05 12:15:49 by joaoteix          #+#    #+#             */
/*   Updated: 2022/11/18 16:58:26 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdint.h>
#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	memlen;
	void	*allocmem;

	if (nmemb > 0 && size > (SIZE_MAX / nmemb))
		return (NULL);
	memlen = nmemb * size;
	allocmem = malloc(memlen);
	if (allocmem)
		ft_bzero(allocmem, memlen);
	return (allocmem);
}
