/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leborges <leborges@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 17:30:28 by leborges          #+#    #+#             */
/*   Updated: 2023/05/12 19:09:15 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <minishell.h>

void delete_token(void *token)
{
	free(((t_token *)token)->str);
	free(token);
}

int	main(int argc, char *argv[], char const *envp[])
{
	char	*input;
	t_scontext	ctx;
	t_list	*tokens, *iter;
	t_list	*pipe_list;

	//ctx.envp = envp;
	(void)ctx;
	(void)argv;
	(void)envp;
	pipe_list = NULL;
	if (argc == 1)
	{
		input = readline(PROMPT);
		while (input)
		{
			tokens = iter = split_tokens(input);
			parse_tokens(tokens, pipe_list);
//			while (iter)
//			{
//				printf("Data: %s | Token: %u\n", ((t_token *)iter->content)->str, ((t_token *)iter->content)->type);
//				iter = iter->next;
//			}
			ft_lstclear(&tokens, delete_token);
			add_history(input);
			free(input);
			input = readline(PROMPT);
		}
		rl_clear_history();
//		env_cmd(&ctx);
//		pwd_cmd();
//		echo_cmd((char *[]){"ATMD4\n", NULL}, 0);
//		echo_cmd((char *[]){"ATMD4SEMESPACO\n", "okboafa", NULL}, 0);
//		echo_cmd((char *[]){"ATMD4SEMESPACO", "okboafa", NULL}, 1);
//		echo_cmd((char *[]){"ATMD4\n", NULL}, 0);
	}
}
