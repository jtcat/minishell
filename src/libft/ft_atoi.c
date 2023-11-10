/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 18:46:50 by joaoteix          #+#    #+#             */
/*   Updated: 2023/11/10 14:58:24 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_isspace(char c)
{
	return (c == ' ' || (c >= '\t' && c <= '\r'));
}

static int	ft_is_sign(char c)
{
	return (c == '+' || c == '-');
}

int	ft_atoi(const char *str, int *isval)
{
	int	numb;
	int	sign;

	numb = 0;
	sign = 1;
	while (ft_isspace(*str))
		str++;
	if (ft_is_sign(*str))
	{
		sign *= (*str == '-') * -2 + 1;
		str++;
	}
	while (ft_isdigit(*str))
	{
		numb *= 10;
		numb += (int)(*str - '0');
		str++;
	}
	if (isval)
		*isval = !*str;
	return (sign * numb);
}
