/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leborges <leborges@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 17:30:28 by leborges          #+#    #+#             */
/*   Updated: 2023/11/07 15:26:41 by joaoteix         ###   ########.fr       */
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
#include <sig.h>
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

void	del_cmd(void *content)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)content;
	ft_lstclear(&cmd->args, NULL);
	if (cmd->hd_fd != -1)
		close(cmd->hd_fd);
	if (cmd->redirs)
		ft_lstclear(&cmd->redirs, NULL);
	if (cmd->cmdpath)
		free(cmd->cmdpath);
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
	close(ctx->std_fds[0]);
	close(ctx->std_fds[1]);
	if (!ctx->subshell)
		ft_putstr_fd("exit\n", STDERR_FILENO);
}

int	g_exit_val = 0;

int	main(int argc, char **argv, char const *envp[])
{
	t_shctx	ctx;

	(void)argc;
	(void)argv;
	ft_memset(&ctx, 0, sizeof(t_shctx));
	bind_interact_sigs();
	init_envp(&ctx, envp);
	ctx.input = readline(MSH_CMD_PROMPT);
	while (ctx.input)
	{
		if (!is_blank_str(ctx.input))
		{
			ctx.tokens = split_tokens(ctx.input);
			if (parse_input(&ctx, ctx.tokens, &ctx.cmd_list))
				exec_cmdlist(&ctx, ctx.cmd_list);
			ft_lstclear(&ctx.tokens, del_token);
			ft_lstclear(&ctx.cmd_list, del_pipe);
			add_history(ctx.input);
		}
		free(ctx.input);
		ctx.input = readline(MSH_CMD_PROMPT);
	}
	sctx_destroy(&ctx);
	exit(g_exit_val);
}
