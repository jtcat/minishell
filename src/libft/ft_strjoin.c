/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/05 17:55:51 by joaoteix          #+#    #+#             */
/*   Updated: 2022/11/09 20:38:17 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strjoin(const char *s1, const char *s2)
{
	char	*newstr;
	size_t	i;

	if (!s1 || !s2)
		return (NULL);
	newstr = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!newstr)
		return (NULL);
	i = 0;
	while (*s1)
		newstr[i++] = *(s1++);
	while (*s2)
		newstr[i++] = *(s2++);
	newstr[i] = 0;
	return (newstr);
}
