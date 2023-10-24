/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leborges <leborges@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 17:30:25 by leborges          #+#    #+#             */
/*   Updated: 2023/10/24 13:16:05 by jcat             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <limits.h>
# include <stdbool.h>
# include <libft.h>

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
	char			**envp;
	char			*input;
	t_list			*exports;
	size_t			envp_len;
	t_list			*cmd_list;
	t_list			*tokens;
	unsigned char	cmd_status;
}	t_shctx;

typedef struct s_cmd
{
	t_list	*args;
	t_list	*redirs;
	t_list	*hd_input;
	int		arg_n;
}	t_cmd;

typedef struct s_ppline
{
	t_list			*cmds;
	t_token_type	op;
}	t_ppline;

// Common
void	del_cmd(void *content);

// Lexer entry point
t_list	*split_tokens(char *str);

// Parser entry point
bool	parse_input(t_list *input, t_list **pipe_list);

// Executer entry point
void	exec_cmdlist(t_shctx *ctx, t_list *ppline_lst);

// Builtins
int		pwd_cmd(t_shctx *ctx, char **vars);
int		env_cmd(t_shctx *ctx, char **vars);
int		echo_cmd(t_shctx *ctx, char **args);
int		export_cmd(t_shctx *ctx, char **vars);
int		unset_cmd(t_shctx *ctx, char **var_ids);
int		cd_cmd(t_shctx *ctx, char **args);
int		exit_cmd(t_shctx *ctx, char **args);
#endif
