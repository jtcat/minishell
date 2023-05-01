/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leborges <leborges@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 17:30:28 by leborges          #+#    #+#             */
/*   Updated: 2023/04/28 14:41:04 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <minishell.h>

void delete_token(void *token)
{
	free(((t_token *)token)->string);
	free(token);
}

int	main(int argc, char *argv[], char const *envp[])
{
	char	*input;
	t_scontext	ctx;
	t_list	*list, *iter;

	ctx.envp = envp;
	(void)argv;
	if (argc == 1)
	{
		input = readline(PROMPT);
		while (input)
		{
			list = iter = split_tokens(input);
			while (iter)
			{
				printf("Data: %s | Token: %u\n", ((t_token *)iter->content)->string, ((t_token *)iter->content)->type);
				iter = iter->next;
			}
			ft_lstclear(&list, delete_token);
			add_history(input);
			free(input);
			input = readline(PROMPT);
		}
		rl_clear_history();
		env_cmd(&ctx);
		pwd_cmd();
		echo_cmd((char *[]){"ATMD4\n", NULL}, 0);
		echo_cmd((char *[]){"ATMD4SEMESPACO\n", "okboafa", NULL}, 0);
		echo_cmd((char *[]){"ATMD4SEMESPACO", "okboafa", NULL}, 1);
		echo_cmd((char *[]){"ATMD4\n", NULL}, 0);
	}
}
