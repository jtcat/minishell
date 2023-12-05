/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 23:37:11 by joaoteix          #+#    #+#             */
/*   Updated: 2023/12/05 15:12:51 by jcat             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include <minishell.h>

typedef int	(*t_builtin_func)(t_shctx *, char **);

t_builtin_func	get_builtinfunc(t_cmd *cmd);
char			**expand_args(t_shctx *ctx, t_cmd *cmd);
char			*expand_word(t_shctx *ctx, char **word_ref, int varonly);
void			resolve_cmd(t_shctx *ctx, char **cmd_path_ref);
int				resolve_redirs(t_shctx *ctx,
					t_cmd *cmd, int pipefd[2], int piperfd);
void			handle_exec_err(t_shctx *ctx, char **args,
					char **envp, char *cmdpath);
char			**conv_llenvp(size_t envp_len, t_dlist *envp);
void			save_io(pid_t std_fds[2]);
void			restore_io(pid_t std_fds[2]);
int				stop_cmd(t_shctx *ctx, int pid);

// Executer entry point
void			exec_cmdlist(t_shctx *ctx, t_list *ppline_lst);
#endif
