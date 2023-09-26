/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_procnumb.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/16 03:47:01 by joaoteix          #+#    #+#             */
/*   Updated: 2022/12/29 06:41:20 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

void	ft_putpadding(char c, size_t len, int fd)
{
	while (len-- > 0)
		ft_putchar_fd(c, fd);
}

size_t	getnumlen(unsigned long int num, int base_size)
{
	size_t	len;

	len = 1;
	while (num / base_size)
	{
		num /= base_size;
		len++;
	}
	return (len);
}

size_t	ft_max_size_t(size_t n1, size_t n2)
{
	if (n1 > n2)
		return (n1);
	return (n2);
}

void	ft_procnumbspec(t_formspec *specs, unsigned long int num, int sign)
{
	if (specs->type == 'x')
		specs->base = "0123456789abcdef";
	else if (specs->type == 'X')
		specs->base = "0123456789ABCDEF";
	specs->base_size = ft_strlen(specs->base);
	specs->numlen = getnumlen(num, specs->base_size) \
					* !(num == 0 && specs->precision == 0);
	specs->zeros = (specs->numlen < specs->precision) \
					* (specs->precision - specs->numlen);
	if (specs->type == 'x' && num > 0 && specs->flags['#'])
		specs->prefix = "0x";
	else if (specs->type == 'X' && num > 0 && specs->flags['#'])
		specs->prefix = "0X";
	else if (specs->flags['+'] && sign > 0)
		specs->prefix = "+";
	else if (specs->flags[' '] && sign > 0)
		specs->prefix = " ";
	else if (sign < 0)
		specs->prefix = "-";
	specs->field_w = ft_max_size_t(specs->field_w, specs->zeros \
					+ specs->numlen + ft_strlen(specs->prefix));
	if (specs->flags['0'])
		specs->zeros = specs->field_w - specs->numlen \
						- ft_strlen(specs->prefix);
}

void	ft_procnumb(t_formspec *specs, unsigned long int num, int sign)
{
	size_t	padding;
	char	numb_str[21];

	ft_procnumbspec(specs, num, sign);
	padding = specs->field_w - specs->numlen - ft_strlen(specs->prefix) \
		- specs->zeros;
	if (!specs->flags['-'])
		ft_putpadding(' ', padding, specs->fd);
	ft_putstr_fd(specs->prefix, specs->fd);
	ft_putpadding('0', specs->zeros, specs->fd);
	numb_str[specs->numlen] = '\0';
	while (specs->numlen-- > 0)
	{
		numb_str[specs->numlen] = specs->base[num % specs->base_size];
		num /= specs->base_size;
	}
	ft_putstr_fd(numb_str, specs->fd);
	if (specs->flags['-'])
		ft_putpadding(' ', padding, specs->fd);
}
