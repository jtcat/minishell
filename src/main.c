/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leborges <leborges@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 17:30:28 by leborges          #+#    #+#             */
/*   Updated: 2023/10/29 19:50:43 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dl_list.h"
#include <stdlib.h>
#include <minishell.h>
#include <shell_utils.h>
#include <exec.h>
#include <parser.h>
#include <lexer.h>
#include <env.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <unistd.h>

void	del_token(void *token)
{
	free(((t_token *)token)->str);
	free(token);
}

void	do_nothing(void *content)
{
	(void)content;
	return ;
}

void	del_cmd(void *content)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)content;
	ft_lstclear(&cmd->args, do_nothing);
	if (cmd->hd_input)
		ft_lstclear(&cmd->hd_input, free);
	if (cmd->redirs)
		ft_lstclear(&cmd->redirs, do_nothing);
	if (cmd->cmdname)
		free(cmd->cmdname);
	free(cmd);
}

void	del_pipe(void *content)
{
	t_ppline	*pipeline;

	pipeline = (t_ppline *)content;
	ft_lstclear(&pipeline->cmds, del_cmd);
	free(pipeline);
}

void	sctx_destroy(t_shctx *ctx)
{
	if (ctx->input)
		free(ctx->input);
	ft_dlstclear(&ctx->exports, free);
	ft_dlstclear(&ctx->envp, free);
	ft_lstclear(&ctx->tokens, del_token);
	ft_lstclear(&ctx->cmd_list, del_pipe);
	rl_clear_history();
}

int	g_exit_val = 0;

int	main(int argc, char *argv[], char const *envp[])
{
	t_shctx	ctx;

	(void)argv;
	ctx.input = NULL;
	ctx.cmd_list = NULL;
	ctx.tokens = NULL;
	ctx.cmd_status = 0;
	ctx.subshell = false;
	init_envp(&ctx, envp);
	if (argc == 1)
	{
		ctx.input = readline(MSH_CMD_PROMPT);
		while (ctx.input)
		{
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
	}
	return (g_exit_val);
}
