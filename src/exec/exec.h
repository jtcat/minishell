/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 23:37:11 by joaoteix          #+#    #+#             */
/*   Updated: 2023/09/27 00:05:51 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"

typedef int	(*t_builtin_func)(t_shctx *, char **);

t_builtin_func	get_builtinfunc(t_cmd *cmd);
char			**expand_args(t_shctx *ctx, t_cmd *cmd, char ***args);
char			*expand_word(t_shctx *ctx, char **word_ref);
int				resolve_cmd(t_shctx *ctx, char **cmd_path_ref);
int				resolve_redirs(t_shctx *ctx,
					t_cmd *cmd, int pipefd[2], int piperfd);
#endif
