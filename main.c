/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leborges <leborges@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 17:30:28 by leborges          #+#    #+#             */
/*   Updated: 2023/04/26 21:01:34 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	main(int argc, char *argv[], char const *envp[])
{
	char	*input;
	t_scontext	ctx;
	t_list	*list;

	ctx.envp = envp;
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
		env_cmd(&ctx);
		pwd_cmd();
		echo_cmd((char *[]){"ATMD4\n", NULL}, 0);
		echo_cmd((char *[]){"ATMD4SEMESPACO\n", "okboafa", NULL}, 1);
		echo_cmd((char *[]){"ATMD4\n", NULL}, 0);
		rl_clear_history();
	}
}
