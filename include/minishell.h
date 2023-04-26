/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leborges <leborges@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 17:30:25 by leborges          #+#    #+#             */
/*   Updated: 2023/04/26 02:24:19 by joaoteix         ###   ########.fr       */
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

enum Token_type {NEW_LINE, NANE, ASSIGNMENT, WORD};

# define PROMPT "minishell >"

typedef struct s_scontext
{
	char const 	**envp;
	char const 	**svars;
	size_t	envp_len;
}	t_scontext;

// Builtins
int	pwd(void);
int env(t_scontext *ctx);
int	echo(char str[], int opt_n);
	
#endif
