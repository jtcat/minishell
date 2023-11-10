/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expan.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 00:11:03 by joaoteix          #+#    #+#             */
/*   Updated: 2023/11/10 20:52:35 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <shell_utils.h>
#include <env.h>
#include "exec.h"

extern int	g_exit_val;

char	*str_cat(char **dst_ref, char *src)
{
	char	*tmp;

	tmp = ft_strjoin(*dst_ref, src);
	free(src);
	free(*dst_ref);
	*dst_ref = tmp;
	return (*dst_ref);
}

// 'echo $' should result in '$\n'
// 'echo $+' should result in '$+\n'
char	*expand_var(t_shctx *ctx, char *cursor, char **expan, char **word_start)
{
	char const	*start = ++cursor;
	char		*tmp;
	char		*var_val;

	str_cat(expan, ft_substr(*word_start, 0, cursor - *word_start - 1));
	if (*cursor == '?')
	{
		str_cat(expan, ft_itoa(g_exit_val));
		*word_start = ++cursor;
		return (cursor);
	}
	while (ft_isalnum(*cursor) || *cursor == '_')
		cursor++;
	*word_start = cursor - 1;
	if (cursor == start)
		return (cursor);
	*word_start = cursor;
	var_val = get_var_val(ctx, start);
	if (!var_val)
		return (cursor);
	tmp = ft_strjoin(*expan, var_val);
	free(*expan);
	*expan = tmp;
	return (cursor);
}

char	*expand_dquote(t_shctx *ctx, char **word_start, 
		char *cursor, char **expan)
{
	str_cat(expan, ft_substr(*word_start, 0, cursor - *word_start));
	cursor++;
	*word_start = cursor;
	cursor = ft_strchr(cursor, *(cursor - 1));
	if (!cursor)
	{
		(*word_start)--;
		return ((*word_start) + 1);
	}
	if (*(*word_start - 1) == '"')
	{
		cursor = *word_start;
		while (*cursor != '"')
		{
			if (*cursor == '$')
				cursor = expand_var(ctx, cursor, expan, word_start);
			else
				cursor++;
		}
	}
	str_cat(expan, ft_substr(*word_start, 0, cursor - *word_start));
	*word_start = cursor + 1;
	return (cursor + 1);
}

// Word unquoting and parameter expasion.
// This is garbage and barely works
char	*expand_word(t_shctx *ctx, char **word_ref, int varonly)
{
	char	*cursor;
	char	*word_start;
	char	*expan;

	cursor = *word_ref;
	word_start = *word_ref;
	expan = ft_strdup("");
	while (*cursor)
	{
		if (!varonly && (*cursor == '\'' || *cursor == '"'))
			cursor = expand_dquote(ctx, &word_start, cursor, &expan);
		else if (*cursor == '$')
			cursor = expand_var(ctx, cursor, &expan, &word_start);
		else
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
		expand_word(ctx, arg_iter->content, 0);
		args[i++] = *(char **)arg_iter->content;
		arg_iter = arg_iter->next;
	}
	return (args);
}
