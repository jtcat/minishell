/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 15:15:56 by joaoteix          #+#    #+#             */
/*   Updated: 2023/05/05 21:07:24 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	is_ctrl_op(t_token token)
{
	return (!token && token->type == pipe);
}

void	parse_cmd(t_list *tokens, t_list *cmd_list)
{
	t_cmd	*cmd;

	cmd = new_cmd();
	cmd->pipe_in == cmd_list->content->pipe_out
	while (!is_ctrl_op(tokens->content) && ft_strchar(tokens->content->str, '='))
	{
		ft_lstadd_back(cmd->exports, ft_lstnew(tokens->content));
		tokens = tokens->next;
	}
	while (!is_ctrl_op(tokens->content))
	{
		if (tokens->content->type == red_in)
			cmd->red_in = tokens->next->content->str;
		else if (tokens->content->type == red_out)
			cmd->red_out = tokens->next->content->str;
		else if (tokens->content->type == here_doc)
			cmd->hd_delimit = tokens->next->content->str;
		else
			ft_lstadd_back(cmd->args, ft_lstnew(tokens->content->str));
		if (tokens->content->type == red_in || tokens->content->type == red_out
				|| tokens->content->type == red_out
				|| tokens->content->type == here_doc)
			tokens = tokens->next;
		tokens = tokens->next;
	}
	ft_lstadd_back(cmd_list, cmd);
}

void	parse_pipeline(t_list *tokens, t_list *cmds)
{
	while (tokens)
	{
		if (tokens->content->type == pipe)
			cmds->pipe_out = 1;
	}
}

t_list	*parse_tokens(t_list	*tokens_head)
{
	t_list	*cmd_list;

	cmd_list = NULL;
	return (cmd_list);
}
