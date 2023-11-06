/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/20 13:25:57 by joaoteix          #+#    #+#             */
/*   Updated: 2023/11/06 13:04:24 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gnl.h"
#include <stdint.h>
#include <stddef.h>

void	readtobuff(int fd, t_filebuff *buff)
{
	buff->len = read(fd, buff->data, BUFFER_SIZE);
	buff->head = 0;
}

t_filebuff	*getbuff(int fd, t_filebuff **bufflist)
{
	if (fd == -1)
		return (NULL);
	if (!bufflist[fd])
	{
		bufflist[fd] = malloc(sizeof(t_filebuff));
		bufflist[fd]->len = 0;
		bufflist[fd]->head = 0;
	}
	return (bufflist[fd]);
}

void	clearbuff(int fd, t_filebuff **bufflist)
{
	free(bufflist[fd]);
	bufflist[fd] = NULL;
}

ssize_t	parse_buff(char *buff, char *end, char **line, size_t *linelen)
{
	char	*newline;
	size_t	i;

	i = 0;
	while ((buff + i) < end)
	{
		if (buff[i++] == '\n')
			break ;
	}
	newline = ft_calloc(*linelen + i + 1, sizeof(char));
	if (!newline)
		return (-1);
	if (line)
	{
		ft_memcpy(newline, *line, *linelen * sizeof(char));
		free(*line);
	}
	ft_memcpy(newline + *linelen, buff, i * sizeof(char));
	*linelen += i;
	newline[*linelen] = '\0';
	*line = newline;
	return (i);
}

char	*get_next_line(int fd)
{
	static t_filebuff	*bufflist[2000] = {NULL};
	t_filebuff			*buff;
	char				*line;
	size_t				linelen;

	line = NULL;
	linelen = 0;
	buff = getbuff(fd, bufflist);
	if (!buff)
		return (NULL);
	if (buff->head == buff->len)
		readtobuff(fd, buff);
	while (buff->len > 0 && (!line || line[linelen - 1] != '\n'))
	{
		if (buff->head == buff->len)
			readtobuff(fd, buff);
		buff->head += parse_buff(buff->data + buff->head, buff->data\
			   	+ buff->len, &line, &linelen);
	}
	if (buff->len == 0)
		clearbuff(fd, bufflist);
	return (line);
}
