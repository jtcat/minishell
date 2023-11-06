/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hdoc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 17:30:59 by joaoteix          #+#    #+#             */
/*   Updated: 2023/11/06 17:29:30 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <sig.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <unistd.h>

#define HD_WARN "warning: here-document delimited by end-of-file (wanted `%s')\n"

extern int	g_exit_val;

void	handle_hd_int(int sig)
{
	g_exit_val = 128 + sig;
	ft_putchar_fd('\n', STDOUT_FILENO);
	close(STDIN_FILENO);
}

void	read_hd(char *delim, int pipefd[2])
{
	char		*line;

	close(pipefd[0]);
	line = readline(HD_PROMPT);
	while (line)
	{
		if (ft_strcmp(line, delim) == 0 && line[ft_strlen(delim)] == '\0')
			break ;
		ft_putstr_fd(line, pipefd[1]);
		ft_putchar_fd('\n', pipefd[1]);
		free(line);
		line = readline(HD_PROMPT);
	}
	if (line)
		free(line);
	close(pipefd[1]);
	if (!line && g_exit_val == 0)
		ft_dprintf(STDERR_FILENO, MSH_ERR_PFIX HD_WARN, delim);
}

// Returns read fd to buffer containing here_doc input
// TODO: Handle exit code
int	exec_hd(t_shctx *ctx, t_cmd *cmd, char *delim)
{
	pid_t	pid;
	int		pipefd[2];
	int		stat;

	pipe(pipefd);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		g_exit_val = 0;
		signal(SIGINT, handle_hd_int);
		read_hd(delim, pipefd);
		sctx_destroy(ctx);
		exit(g_exit_val);
	}
	close(pipefd[1]);
	cmd->hd_fd = pipefd[0];
	waitpid(pid, &stat, 0);
	g_exit_val = WEXITSTATUS(stat);
	bind_interact_sigs();
	return (g_exit_val);
}
