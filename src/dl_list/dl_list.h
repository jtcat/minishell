/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dl_list.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcat <joaoteix@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 17:21:55 by jcat              #+#    #+#             */
/*   Updated: 2023/10/30 12:55:41 by jcat             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DL_LIST_H
# define DL_LIST_H

typedef struct s_dlist
{
	void			*content;
	struct s_dlist	*next;
	struct s_dlist	*prev;
}	t_dlist;

// Methods
t_dlist	*ft_dlstnew(void *content);
t_dlist	*ft_dlstlast(t_dlist *lst);
t_dlist	*ft_dlstdup(t_dlist *lst);
void	ft_dlstadd_back(t_dlist **lst, t_dlist *new);
void	ft_dlstdelone(t_dlist *lst, void (*del)(void *));
void	ft_dlstrmone(t_dlist **headref, t_dlist *node, void (*del)(void *));
void	ft_dlstclear(t_dlist **node, void (*del)(void *));
#endif
