/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leborges <leborges@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 17:30:28 by leborges          #+#    #+#             */
/*   Updated: 2023/04/25 18:13:48 by leborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Minishell.h>

int	main()
{
	char	*input;
	t_list	*list;

	input = readline("Super mini shellings$ ");
	list = split_tokens(input);

	while (list)
	{
		printf("Data: %s | Token: %u\n", ((t_token *)list->content)->string, ((t_token *)list->content)->type);
		list = list->next;
	}

	/*while (input)
	{
		input = readline("Super mini shellings$ ");
		add_history(input);
		rl_redisplay();
	}*/
}
