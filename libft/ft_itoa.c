/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/05 19:15:05 by joaoteix          #+#    #+#             */
/*   Updated: 2022/11/06 13:05:54 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>
#include "libft.h"

static size_t	getdigitn(int n)
{
	size_t	digits;

	digits = 1;
	while (n / 10)
	{
		digits++;
		n /= 10;
	}
	return (digits);
}

char	*ft_itoa(int n)
{
	char	*str;
	size_t	strn;
	size_t	i;

	strn = getdigitn(n) + (n < 0);
	str = ft_calloc(strn + 1, sizeof(char));
	if (!str)
		return (NULL);
	if (n < 0)
		str[0] = '-';
	else
		n = -n;
	str[strn - 1] = -(char)(n % 10) + '0';
	n /= 10;
	i = 2;
	while (n)
	{
		str[strn - i] = -(char)(n % 10) + '0';
		n /= 10;
		i++;
	}
	str[strn] = '\0';
	return (str);
}
