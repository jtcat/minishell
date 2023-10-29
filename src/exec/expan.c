/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expan.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 00:11:03 by joaoteix          #+#    #+#             */
/*   Updated: 2023/10/29 18:26:52 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <shell_utils.h>
#include <env.h>
#include "exec.h"

extern int	g_exit_val;

void	str_cat(char **dst_ref, char *src)
{
	char	*tmp;

	tmp = ft_strjoin(*dst_ref, src);
	free(src);
	free(*dst_ref);
	*dst_ref = tmp;
}

// 'echo $' should result in '$\n'
// 'echo $+' should result in '$+\n'
char	*expand_var(t_shctx *ctx, char *cursor, char **expansion)
{
	char const	*start = cursor;
	char		*tmp;
	char		*var_val;

	if (*cursor == '?')
	{
		*expansion = ft_itoa(g_exit_val);
		return (cursor + 1);
	}
	while (ft_isalnum(*cursor) || *cursor == '_')
		(cursor)++;
	if (cursor == start)
		return (cursor - 1);
	var_val = get_var_val(ctx, start);
	if (!var_val)
		return (cursor);
	tmp = ft_strjoin(*expansion, ft_strchr(var_val, '=') + 1);
	free(*expansion);
	*expansion = tmp;
	return (cursor);
}

// Word unquoting and parameter expasion.
// This is garbage and barely works
char	*expand_word(t_shctx *ctx, char **word_ref)
{
	char	*cursor;
	char	*word_start;
	char	*expan;

	cursor = *word_ref;
	word_start = *word_ref;
	expan = ft_strdup("");
	while (is_wordchar(*cursor))
	{
		if (*cursor == '\'')
		{
			str_cat(&expan, ft_substr(word_start, 0, cursor - word_start));
			word_start = cursor + 1;
			cursor = ft_strchr(cursor + 1, '\'');
			str_cat(&expan, ft_substr(word_start, 0, cursor - word_start));
			word_start = cursor + 1;
		}
		else if (*cursor == '"')
		{
			str_cat(&expan, ft_substr(word_start, 0, cursor - word_start));
			cursor++;
			word_start = cursor;
			while (*cursor != '"')
			{
				if (*cursor == '$')
				{
					str_cat(&expan,
						ft_substr(word_start, 0, cursor - word_start - 1));
					word_start = expand_var(ctx, cursor + 1, &expan);
				}
				cursor++;
			}
			str_cat(&expan, ft_substr(word_start, 0, cursor - word_start));
			word_start = cursor + 1;
		}
		else if (*cursor == '$')
			word_start = expand_var(ctx, cursor + 1, &expan);
		cursor++;
	}
	str_cat(&expan, ft_substr(word_start, 0, cursor - word_start));
	free(*word_ref);
	*word_ref = expan;
	return (*word_ref);
}

// Returns string array with every argument expanded
char	**expand_args(t_shctx *ctx, t_cmd *cmd)
{
	char	**args;
	t_list	*arg_iter;
	int		i;

	args = malloc(sizeof(char *) * (cmd->arg_n + 1));
	arg_iter = cmd->args->next;
	args[0] = *(char **)cmd->args->content;
	args[cmd->arg_n] = NULL;
	i = 1;
	while (arg_iter)
	{
		args[i++] = expand_word(ctx, arg_iter->content);
		arg_iter = arg_iter->next;
	}
	return (args);
}
