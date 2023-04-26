/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leborges <leborges@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 17:30:25 by leborges          #+#    #+#             */
/*   Updated: 2023/04/26 21:01:52 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stddef.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <libft.h>

enum token_type {NEW_LINE, NANE, OPERATOR, WORD, TOKEN};
typedef enum token_type token_type;

// Lexer
t_list	*split_tokens(char *str);

typedef struct s_token
{
	char			*string;
	enum token_type	type;
}			t_token;

# define PROMPT "minishell >"

typedef struct s_scontext
{
	char const 	**envp;
	char const 	**svars;
	size_t	envp_len;
}	t_scontext;

// Builtins
int	pwd_cmd(void);
int env_cmd(t_scontext *ctx);
int	echo_cmd(char *str[], int opt_n);
	
#endif
