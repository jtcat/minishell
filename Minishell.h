/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leborges <leborges@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 17:30:25 by leborges          #+#    #+#             */
/*   Updated: 2023/04/24 19:46:32 by leborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
#inlcude "libft.h"

enum Token_type{NEWLINE, NANE, ASSIGNMENT, WORD};


typedef struct s_list
{
	char			*data;
	struct s_stack	*next;	
}	
					t_list;
#endif