/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_procmulti.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/16 03:42:08 by joaoteix          #+#    #+#             */
/*   Updated: 2022/12/29 06:36:57 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

void	ft_procchar(t_formspec *specs, int c)
{
	size_t	padding;

	specs->field_w += specs->field_w == 0;
	padding = specs->field_w - 1;
	if (!specs->flags['-'])
		ft_putpadding(' ', padding, specs->fd);
	ft_putchar_fd((unsigned char)c, specs->fd);
	if (specs->flags['-'])
		ft_putpadding(' ', padding, specs->fd);
}

void	ft_procptr(t_formspec *specs, unsigned long int ptr)
{
	if (!ptr)
		return (ft_procstr(specs, "(nil)"));
	specs->flags['#'] = 1;
	specs->type = 'x';
	ft_procnumb(specs, ptr, 1);
}

void	ft_procint(t_formspec *specs, long int numb)
{
	int			sign;

	sign = (numb >= 0) * 2 - 1;
	ft_procnumb(specs, (unsigned long int)numb * sign, sign);
}

void	ft_procuint(t_formspec *specs, unsigned int numb)
{
	ft_procnumb(specs, (unsigned long int)numb, 1);
}

void	ft_procstr(t_formspec *specs, const char *str)
{
	size_t	len;
	size_t	padding;

	if (!str)
	{
		str = "(null)";
		if (specs->flags['.'] && specs->precision < 6)
			specs->precision = 0;
	}
	len = ft_strlen(str);
	if (specs->flags['.'] && len > specs->precision)
		len = specs->precision;
	if (specs->field_w < len)
		specs->field_w = len;
	padding = specs->field_w - len;
	if (!specs->flags['-'])
		ft_putpadding(' ', padding, specs->fd);
	while (len-- > 0)
		ft_putchar_fd(*(str++), specs->fd);
	if (specs->flags['-'])
		ft_putpadding(' ', padding, specs->fd);
}
