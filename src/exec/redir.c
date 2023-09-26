/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 00:08:00 by joaoteix          #+#    #+#             */
/*   Updated: 2023/09/27 00:14:34 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils.h"
#include "exec.h"

#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int	file_redir(t_shctx *ctx, char **fname_ref, int red_type)
{
	int			redir_to;
	char *const	exp_filename = expand_word(ctx, fname_ref);

	if (!ft_strcmp(exp_filename, ""))
	{
		ft_dprintf(STDERR_FILENO, MSH_ERR_PFIX "ambiguous expansion");
		return (-1);
	}
	redir_to = open(exp_filename,
			(red_type == red_in) * (O_RDONLY)
			+ (red_type == red_out) * FILE_TRUNC
			+ (red_type == red_out_ap) * FILE_APPEND,
			ACCESS_BITS);
	if (redir_to < 0)
	{
		ft_dprintf(STDERR_FILENO, MSH_ERR_PFIX "%s: %s\n",
			exp_filename, strerror(errno));
		return (-1);
	}
	dup2(redir_to, (red_type == red_in) * STDIN_FILENO
		+ (red_type == red_out || red_type == red_out_ap)
		* STDOUT_FILENO);
	close(redir_to);
	return (redir_to);
}

void	pipe_redir(int iofd[2], int piperfd)
{
	if (piperfd > 0)
		close(piperfd);
	if (iofd[1] > 0)
	{
		dup2(iofd[1], STDIN_FILENO);
		close(iofd[1]);
	}
	if (iofd[0] > 0)
	{
		dup2(iofd[0], STDOUT_FILENO);
		close(iofd[0]);
	}
}

int	resolve_redirs(t_shctx *ctx, t_cmd *cmd, int pipefd[2], int piperfd)
{
	t_list	*redir;
	t_token	*redir_tok;
	int		redir_stat;

	pipe_redir(pipefd, piperfd);
	redir = cmd->redirs;
	while (redir)
	{
		redir_tok = get_token(&redir);
		if (redir_tok->type == here_doc)
			dup2(cmd->hd_fd, STDIN_FILENO);
		else if (redir_tok->type == red_out
			|| redir_tok->type == red_out_ap
			|| redir_tok->type == red_in)
		{
			redir_stat = file_redir(ctx, &redir_tok->str, redir_tok->type);
			if (redir_stat < 0)
				return (redir_stat);
		}
		redir = redir->next;
	}
	return (0);
}
