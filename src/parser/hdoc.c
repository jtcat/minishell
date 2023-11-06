/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hdoc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 17:30:59 by joaoteix          #+#    #+#             */
/*   Updated: 2023/11/06 13:22:38 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <unistd.h>

#define HD_WARN "warning: here-document by end-of-file (wanted `%s')\n"

void	read_hd(char *delim, pid_t pipefd[2])
{
	char		*line;

	close(pipefd[0]);
	line = readline(HD_PROMPT);
	while (line)
	{
		if (ft_strcmp(line, delim) == 0 && line[ft_strlen(delim)] == '\0')
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, pipefd[1]);
		ft_putchar_fd('\n', pipefd[1]);
		free(line);
		line = readline(HD_PROMPT);
	}
	if (!line)
	{
		ft_dprintf(STDERR_FILENO, MSH_ERR_PFIX HD_WARN, delim);
	}
	close(pipefd[1]);
}

// Returns read fd to buffer containing here_doc input
// TODO: Handle case where here_doc is terminated by EOF
void	exec_hd(t_shctx *ctx, t_cmd *cmd, t_token *delim)
{
	pid_t	pid;
	int		pipefd[2];

	pipe(pipefd);
	pid = fork();
	if (pid == 0)
	{
		read_hd(delim->str, pipefd);
		sctx_destroy(ctx);
		exit(0);
	}
	close(pipefd[1]);
	cmd->hd_fd = pipefd[0];
	waitpid(pid, NULL, 0);
}
