/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlstdup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 16:32:57 by joaoteix          #+#    #+#             */
/*   Updated: 2023/10/29 17:06:55 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dl_list.h"
#include <stdlib.h>

t_dlist	*ft_dlstdup(t_dlist *lst)
{
	t_dlist	*newlst;

	newlst = NULL;
	while (lst)
	{
		ft_dlstadd_back(&newlst, ft_dlstnew(lst->content));
		lst = lst->next;
	}
	return (newlst);
}
