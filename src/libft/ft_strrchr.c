/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 15:55:57 by joaoteix          #+#    #+#             */
/*   Updated: 2022/11/09 21:26:54 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stddef.h>

char	*ft_strrchr(const char *str, int c)
{
	char	*match;

	match = NULL;
	while (*str)
	{
		if (*str == (char)c)
			match = (char *)(str);
		str++;
	}
	if (*str == (char)c)
		match = (char *)(str);
	return (match);
}
