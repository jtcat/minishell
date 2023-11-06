/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leborges <leborges@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 17:30:25 by leborges          #+#    #+#             */
/*   Updated: 2023/11/06 16:58:43 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <limits.h>
# include <stdbool.h>
# include <libft.h>
# include <dl_list.h>

# define MSH_CMD_PROMPT "minishell > "
# define HD_PROMPT "> "
# define MSH_FILE_ERR_MSG "No such file or directory"
# define MSH_ERR_PFIX "minishell: "
# define MSH_CMD_NFOUND_ERR "command not found"
# define MSH_CMD_NPERM_ERR "Permission denied"

# define ACCESS_BITS 420
# define FILE_APPEND 1089
# define FILE_TRUNC	577

enum e_token_type {none, word, name, pipe_op, red_in, red_out, \
	red_out_ap, here_doc, lst_and, lst_or, lst_no_op, newline};
typedef enum e_token_type	t_token_type;

typedef struct s_token
{
	char			*str;
	t_token_type	type;
}	t_token;

typedef struct s_shctx
{
	bool			subshell;
	char			*input;
	t_dlist			*exports;
	t_dlist			*envp;
	size_t			exports_len;
	size_t			envp_len;
	t_list			*cmd_list;
	t_list			*tokens;
	unsigned char	cmd_status;
}	t_shctx;

typedef struct s_cmd
{
	char	*cmdpath;
	t_list	*args;
	t_list	*redirs;
	int		hd_fd;
	int		arg_n;
}	t_cmd;

typedef struct s_ppline
{
	t_list			*cmds;
	t_token_type	op;
}	t_ppline;

// Common
void	del_cmd(void *content);

// Shell Context
void	sctx_destroy(t_shctx *ctx);

// Builtins
int		pwd_cmd(t_shctx *ctx, char **vars);
int		env_cmd(t_shctx *ctx, char **vars);
int		echo_cmd(t_shctx *ctx, char **args);
int		export_cmd(t_shctx *ctx, char **vars);
int		unset_cmd(t_shctx *ctx, char **var_ids);
int		cd_cmd(t_shctx *ctx, char **args);
int		exit_cmd(t_shctx *ctx, char **args);
#endif
