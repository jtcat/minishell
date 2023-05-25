/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leborges <leborges@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 17:30:28 by leborges          #+#    #+#             */
/*   Updated: 2023/05/25 20:19:20 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <minishell.h>
#include "libft.h"
#include "utils.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

void del_token(void *token)
{
	free(((t_token *)token)->str);
	free(token);
}

void do_nothing(void *content)
{
	(void)content;
	return ;
}

void	del_cmd(void *content)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)content;
	ft_lstclear(&cmd->args, do_nothing);
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
	ft_lstclear(&pipeline->cmds, del_cmd);
	free(pipeline);
}

void	sctx_destroy(t_scontext *ctx)
{
	free_ptrarr((void **)ctx->envp, free);
	ft_lstclear(&ctx->tokens, del_token);
	ft_lstclear(&ctx->cmd_list, del_pipe);
	rl_clear_history();
}

char	**dup_envp(char const *envp[])
{
	char	**new_envp;
	int		len;

	len = 0;
	while (envp[len])
		len++;
	new_envp = malloc(sizeof(char *) * (len + 1));
	new_envp[len] = NULL;
	while (len-- > 0)
		new_envp[len] = ft_strdup((char *)envp[len]);
	return (new_envp);
}

int	main(int argc, char *argv[], char const *envp[])
{
	t_scontext	ctx;

	//ctx.envp = envp;
	(void)argv;
	ctx.cmd_list = NULL;
	ctx.tokens = NULL;
	ctx.envp = dup_envp(envp);
	if (argc == 1)
	{
		ctx.input = readline(MSH_CMD_PROMPT);
		while (ctx.input)
		{
		//	while (iter)
		//	{
		//		printf("Data: %s | Token: %u\n", ((t_token *)iter->content)->str, ((t_token *)iter->content)->type);
		//		iter = iter->next;
		//	}
			if (!is_blank_str(ctx.input))
			{
				ctx.tokens = split_tokens(ctx.input);
				if (parse_input(ctx.tokens, &ctx.cmd_list))
					exec_cmdlist(&ctx, ctx.cmd_list);
				ft_lstclear(&ctx.tokens, del_token);
				ft_lstclear(&ctx.cmd_list, del_pipe);
				add_history(ctx.input);
			}
			free(ctx.input);
			ctx.input = readline(MSH_CMD_PROMPT);
		}
		sctx_destroy(&ctx);
//		env_cmd(&ctx);
//		pwd_cmd();
//		echo_cmd((char *[]){"ATMD4\n", NULL}, 0);
//		echo_cmd((char *[]){"ATMD4SEMESPACO\n", "okboafa", NULL}, 0);
//		echo_cmd((char *[]){"ATMD4SEMESPACO", "okboafa", NULL}, 1);
//		echo_cmd((char *[]){"ATMD4\n", NULL}, 0);
	}
}