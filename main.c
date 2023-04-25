/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leborges <leborges@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 17:30:28 by leborges          #+#    #+#             */
/*   Updated: 2023/04/25 11:27:54 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Minishell.h"
#include "libft.h"
#include <readline/history.h>
#include <readline/readline.h>

int	main()
{
	char	*input;

	input = readline("Super mini shellings$ ");
	while (input)
	{
		input = readline("Super mini shellings$ ");
		add_history(input);
		rl_redisplay();
	}
}
