/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 18:41:32 by joaoteix          #+#    #+#             */
/*   Updated: 2022/11/06 18:53:37 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stddef.h>

typedef const unsigned char	t_cuc;

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (((t_cuc *)s1)[i] == ((t_cuc *)s2)[i] && i < (n - 1))
		i++;
	return (((t_cuc *)s1)[i] - ((t_cuc *)s2)[i]);
}
