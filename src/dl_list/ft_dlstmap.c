/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlstmap.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcat </var/spool/mail/jcat>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 02:20:45 by jcat              #+#    #+#             */
/*   Updated: 2023/10/29 16:24:43 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "dl_list.h"
#include <stdlib.h>

t_dlist	*ft_dlstmap(t_dlist *lst, void *(*f)(void *), void (*del)(void *))
{
	t_dlist	*newlst;
	t_dlist	*newlsthead;

	if (!(lst && f && del))
		return (NULL);
	newlsthead = ft_dlstnew(f(lst->content));
	newlst = newlsthead;
	while (lst->next)
	{
		newlst->next = ft_dlstnew(f(lst->next->content));
		if (!newlst)
		{
			ft_dlstclear(&newlsthead, del);
			return (NULL);
		}
		newlst = newlst->next;
		lst = lst->next;
	}
	return (newlsthead);
}
