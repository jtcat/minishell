/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leborges <leborges@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 17:30:28 by leborges          #+#    #+#             */
/*   Updated: 2023/04/25 20:23:29 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Minishell.h>

int	main()
{
	char	*input;

	input = readline(PROMPT);
	while (input)
	{
	//	if (is_blank_line(input))
	//		free(input);
	//	else
		add_history(input);
		free(input);
		input = readline(PROMPT);
	}
	rl_clear_history();
}
