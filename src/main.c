/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leborges <leborges@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 17:30:28 by leborges          #+#    #+#             */
/*   Updated: 2023/05/19 14:46:47 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <minishell.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

void delete_token(void *token)
{
	free(((t_token *)token)->str);
	free(token);
}

void	del_cmd(void *content)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)content;
	ft_lstclear(&cmd->args, free);
		if (cmd->red_in)
		free(cmd->red_in);
	if (cmd->red_out)
		free(cmd->red_out);
	if (cmd->hd_delim)
		free(cmd->hd_delim);
}

void	del_pipe(void *content)
{
	t_ppline	*pipeline;

	pipeline = (t_ppline *)content;
	ft_lstclear(&pipeline, del_cmd);
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
//		input = readline(MSH_CMD_PROMPT);
//		while (input)
//		{
//			tokens = iter = split_tokens(input);
//			while (iter)
//			{
//				printf("Data: %s | Token: %u\n", ((t_token *)iter->content)->str, ((t_token *)iter->content)->type);
//				iter = iter->next;
//			}
//			parse_input(tokens, &pipe_list);
//			ft_lstclear(&pipe_list, del_pipe);
//			add_history(input);
//			free(input);
//			input = readline(MSH_CMD_PROMPT);
//		}
//		rl_clear_history();
		if (access("minishell", X_OK) != 0)
			perror(MSH_ERR_PFIX);
//		env_cmd(&ctx);
//		pwd_cmd();
//		echo_cmd((char *[]){"ATMD4\n", NULL}, 0);
//		echo_cmd((char *[]){"ATMD4SEMESPACO\n", "okboafa", NULL}, 0);
//		echo_cmd((char *[]){"ATMD4SEMESPACO", "okboafa", NULL}, 1);
//		echo_cmd((char *[]){"ATMD4\n", NULL}, 0);
	}
}
