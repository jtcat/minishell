/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/05 21:22:51 by joaoteix          #+#    #+#             */
/*   Updated: 2022/11/09 20:36:47 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <unistd.h>

void	ft_putnbr_fd(int nb, int fd)
{
	char	str_nb[10];
	int		i;

	i = 9;
	if (nb < 0)
		write(fd, "-", 1);
	else
		nb = -nb;
	while (nb / 10)
	{
		str_nb[i--] = -(nb % 10) + '0';
		nb /= 10;
	}
	str_nb[i] = -(nb % 10) + '0';
	write(fd, str_nb + i, 10 - i);
}
