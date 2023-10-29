/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlstrmone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcat <joaoteix@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 17:33:44 by jcat              #+#    #+#             */
/*   Updated: 2023/10/29 17:18:01 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dl_list.h"

void	ft_dlstrmone(t_dlist **headref, t_dlist *node, void (*del)(void *))
{
	if (!*headref || !node)
		return ;
	if (*headref == node)
		*headref = node->next;
	if (node->prev)
		node->prev->next = node->next;
	if (node->next)
		node->next->prev = node->prev;
	ft_dlstdelone(node, del);
}
