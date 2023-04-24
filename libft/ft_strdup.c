/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/05 12:27:03 by joaoteix          #+#    #+#             */
/*   Updated: 2022/11/05 12:44:14 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>
#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*newstr;
	size_t	len;

	len = ft_strlen(s) + 1;
	newstr = malloc(len);
	if (newstr == NULL)
		return (NULL);
	while (len-- > 0)
		newstr[len] = s[len];
	return (newstr);
}
