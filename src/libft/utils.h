/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 03:16:49 by joaoteix          #+#    #+#             */
/*   Updated: 2022/12/29 06:40:43 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <stddef.h>
# include <stdlib.h>
# include <stdarg.h>
# include "libft.h"

typedef struct s_formspec
{
	unsigned char	flags[256];
	size_t			field_w;
	size_t			precision;
	size_t			zeros;
	size_t			base_size;
	size_t			numlen;
	int				fd;
	char			*base;
	char			*prefix;
	char			type;
}	t_formspec;

void	ft_procchar(t_formspec *specs, int c);
void	ft_procptr(t_formspec *specs, unsigned long int ptr);
void	ft_procint(t_formspec *specs, long int numb);
void	ft_procuint(t_formspec *specs, unsigned int numb);
void	ft_procstr(t_formspec *specs, const char *str);

void	ft_putpadding(char c, size_t len, int fd);
size_t	getnumlen(unsigned long int num, int base_size);
size_t	ft_max_size_t(size_t n1, size_t n2);

void	ft_procnumbspec(t_formspec *specs, unsigned long int num, int sign);
void	ft_procnumb(t_formspec *specs, unsigned long int num, int sign);
#endif
