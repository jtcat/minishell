/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leborges <leborges@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 17:30:28 by leborges          #+#    #+#             */
/*   Updated: 2023/04/26 20:30:48 by leborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Minishell.h>

int	main(int argc, char *argv[], char *envp[])
{
	char	*input;
	t_list	*list;

	(void)argv;
	if (argc == 1)
	{
		input = readline(PROMPT);
		while (input)
		{
			//	if (is_blank_line(input))
			//		free(input);
			//	else
			add_history(input);
			free(input);
			input = readline(PROMPT);
			list = split_tokens(input);
			while (list)
			{
				printf("Data: %s | Token: %u\n", ((t_token *)list->content)->string, ((t_token *)list->content)->type);
				list = list->next;
			}
		}
		env(envp);
		pwd();
		rl_clear_history();
	}
}
