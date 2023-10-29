/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlstdelone.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcat </var/spool/mail/jcat>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 01:46:22 by jcat              #+#    #+#             */
/*   Updated: 2023/10/29 16:30:23 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "dl_list.h"
#include <stdlib.h>

void	ft_dlstdelone(t_dlist *lst, void (*del)(void *))
{
	if (!lst || !del)
		return ;
	if (lst->content && del)
		del(lst->content);
	free(lst);
}
