/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 18:54:08 by joaoteix          #+#    #+#             */
/*   Updated: 2022/12/29 07:06:01 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

static void	initspecs(t_formspec *specs, int fd)
{
	size_t		i;

	specs->precision = 1;
	specs->zeros = 0;
	specs->field_w = 0;
	specs->prefix = "";
	specs->type = 0;
	specs->base = "0123456789";
	specs->fd = fd;
	i = 0;
	while (i < 256)
		specs->flags[i++] = 0;
}

static char	*ft_parseform(const char *form, t_formspec *specs)
{
	char	*iter;

	iter = (char *)form + 1;
	while (*iter && ft_strrchr("#0- +", *iter))
		specs->flags[(int)*(iter++)] = 1;
	specs->flags[' '] *= !specs->flags['+'];
	specs->flags['0'] *= !specs->flags['-'];
	specs->field_w = (size_t)ft_atoi(iter);
	while (*iter && ft_isdigit(*iter))
		(iter)++;
	if (*iter && *iter == '.')
	{
		specs->flags['.'] = 1;
		specs->flags['0'] = 0;
		specs->precision = ft_atoi(++iter);
	}
	while (*iter && ft_isdigit(*iter))
		iter++;
	if (*iter && ft_strrchr("diuxXcsp%", *iter))
		specs->type = *(iter++);
	else
		return ((char *)++form);
	return (iter);
}

static size_t	ft_subfield(const char **form, va_list args, int fd)
{
	t_formspec	specs;
	size_t		field_w;

	initspecs(&specs, fd);
	*form = ft_parseform(*form, &specs);
	if (!specs.type)
		specs.type = '%';
	if (specs.type == 'c')
		ft_procchar(&specs, va_arg(args, int));
	else if (specs.type == 's')
		ft_procstr(&specs, va_arg(args, const char *));
	else if (specs.type == '%')
		ft_procchar(&specs, '%');
	else if (specs.type == 'p')
		ft_procptr(&specs, va_arg(args, unsigned long int));
	else if (specs.type == 'd' || specs.type == 'i')
		ft_procint(&specs, va_arg(args, int));
	else
		ft_procuint(&specs, va_arg(args, unsigned int));
	field_w = specs.field_w;
	return (field_w);
}

int	ft_dprintf(int fd, const char *format, ...)
{
	size_t	len;
	va_list	args;

	va_start(args, format);
	len = 0;
	while (*format)
	{
		if (*format == '%')
			len += ft_subfield(&format, args, fd);
		else
		{
			ft_putchar_fd(*(format++), fd);
			len++;
		}
	}
	va_end(args);
	return (len);
}
