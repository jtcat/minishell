/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcat </var/spool/mail/jcat>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 02:20:45 by jcat              #+#    #+#             */
/*   Updated: 2022/11/10 16:03:54 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <stdlib.h>

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*newlst;
	t_list	*newlsthead;

	if (!(lst && f && del))
		return (NULL);
	newlsthead = ft_lstnew(f(lst->content));
	newlst = newlsthead;
	while (lst->next)
	{
		newlst->next = ft_lstnew(f(lst->next->content));
		if (!newlst)
		{
			ft_lstclear(&newlsthead, del);
			return (NULL);
		}
		newlst = newlst->next;
		lst = lst->next;
	}
	return (newlsthead);
}
