/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 18:46:50 by joaoteix          #+#    #+#             */
/*   Updated: 2022/11/04 21:23:29 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
static int	ft_isspace(char c)
{
	return (c == ' ' || (c >= '\t' && c <= '\r'));
}

static int	ft_isnum(char c)
{
	return (c >= '0' && c <= '9');
}

static int	ft_is_sign(char c)
{
	return (c == '+' || c == '-');
}

int	ft_atoi(char *str)
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
	while (ft_isnum(*str))
	{
		numb *= 10;
		numb += (int)(*str - '0');
		str++;
	}
	return (sign * numb);
}
