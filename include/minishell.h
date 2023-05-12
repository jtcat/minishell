/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leborges <leborges@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 17:30:25 by leborges          #+#    #+#             */
/*   Updated: 2023/05/12 13:09:15 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stddef.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <libft.h>

# define PROMPT "minishell >"

enum e_token_type {new_line, name, pipe_op, word, red_in, red_out, red_out_ap, here_doc, lst_and, lst_or};
typedef enum e_token_type	t_token_type;

typedef struct s_token
{
	char			*str;
	t_token_type	type;
}			t_token;

typedef struct s_scontext
{
	char const	**envp;
	char const	**svars;
	size_t		envp_len;
}	t_scontext;

typedef struct s_cmd
{
	t_list		*args;
	char const	*red_in;
	char const	*red_out;
	char const	*hd_delim;
}	t_cmd;

// Lexer
t_list	*split_tokens(char *str);

// Builtins
int		pwd_cmd(void);
int		env_cmd(t_scontext *ctx);
int		echo_cmd(char *str[], int opt_n);
#endif
