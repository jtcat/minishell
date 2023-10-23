/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 00:08:00 by joaoteix          #+#    #+#             */
/*   Updated: 2023/10/18 18:50:04 by jcat             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "utils.h"
#include "exec.h"

#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

void	handle_redir_err(char *fname)
{
		ft_dprintf(STDERR_FILENO, MSH_ERR_PFIX "%s: %s\n",
			fname, strerror(errno));
}

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
		handle_redir_err(exp_filename);
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

// Here doc input llist is cleared on shell exit
// and not in this function because this command
// might never run
void	redir_hd(t_shctx *ctx, t_cmd *cmd)
{
	int		pipe_fd[2];
	t_list	*iter;

	pipe(pipe_fd);
	iter = cmd->hd_input;
	while (iter)
	{
		expand_word(ctx, (char **)&iter->content);
		ft_putstr_fd(iter->content, pipe_fd[0]);
		ft_putchar_fd('\n', pipe_fd[0]);
		iter = iter->next;
	}
	close(pipe_fd[0]);
	dup2(pipe_fd[1], STDIN_FILENO);
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
			redir_hd(ctx, cmd);
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
