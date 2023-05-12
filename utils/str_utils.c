/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 23:33:09 by joaoteix          #+#    #+#             */
/*   Updated: 2023/05/12 22:32:19 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	is_blank(char c)
{
	return (c == ' ' || c == '\t');
}

int	is_op(char c)
{
	return (c == '&' || c == '>' || c == '<' || c == '|');
}

int	is_wordchar(char c)
{
	return (c != '\0' && !is_blank(c) && !is_op(c));
}

int	is_blank_str(char str[])
{
	while (*str)
		if (!is_blank(*str++))
				return (0);
	return (1);
}
