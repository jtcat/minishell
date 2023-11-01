/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_doc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 17:30:59 by joaoteix          #+#    #+#             */
/*   Updated: 2023/11/01 18:31:29 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Returns read fd to buffer containing here_doc input
// Should be closed when used
// If delim is unquoted, 

void	exec_hd(t_cmd *cmd, t_token *delim)
{
	pid_t	pid;
	int		pipefd[2];

	pipe(pipefd);
	close(pipefd
	pid = fork();
	if (pid == 0)
	{
		read_hd(delim->string, pipefd);
		exit_shell();
	}
	
}

void	read_hd(char *delim, t_pid pipefd[2])
{
	char		*line;

	close(pipefd[1]);
//	if (cmd->hd_input != NULL)
//		ft_lstclear(&cmd->hd_input, free);
	line = readline(HD_PROMPT);
	while (line)
	{
		if (ft_strcmp(line, delim) == 0 && line[ft_strlen(delim)] == '\0')
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, pipefd[1]);
		//ft_lstadd_back(&cmd->hd_input, ft_lstnew(line));
		line = readline(HD_PROMPT);
	}
	close(pipefd[1]);
	//ft_lstadd_back(&cmd->redirs, ft_lstnew(delimtok));
}
