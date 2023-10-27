/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlstclear.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcat </var/spool/mail/jcat>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 01:50:35 by jcat              #+#    #+#             */
/*   Updated: 2023/10/26 17:39:44 by jcat             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "dl_list.h"
#include <stdlib.h>

void	ft_dlstclear(t_dlist **lst, void (*del)(void *))
{
	t_dlist	*tmp;

	if (!lst || !del)
		return ;
	while (*lst)
	{
		if ((*lst)->content)
			del((*lst)->content);
		tmp = (*lst)->next;
		free(*lst);
		*lst = tmp;
	}
}
