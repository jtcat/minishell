/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlstrmone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcat <joaoteix@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 17:33:44 by jcat              #+#    #+#             */
/*   Updated: 2023/10/26 17:37:40 by jcat             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dl_list.h"

void	ft_dlstrmone(t_dlist *node, void (*del)(void *))
{
	node->prev->next = node->next;
	node->next->prev = node->prev;
	ft_dlstdelone(node, del);
}
