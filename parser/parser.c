/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 15:15:56 by joaoteix          #+#    #+#             */
/*   Updated: 2023/05/10 13:18:40 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
 * Command grammar:
 *
 * simple_cmd: simple_cmd ( '|' simple_cmd)*
 * 			| assigment* (redirect | word)*
 * 			| assignment
 * 			| (redirect | word)
 * 			;
 */

int	is_ctrl_op(t_token token)
{
	return (token && token->type == pipe);
}

int	parse_cmd(t_list *tokens, t_cmd *cmd, int pipe_out[2])
{
	cmd->pipe_in == pipe_out;
	while (*tokens && strchar((*tokens)->content->str, '='));
	{
		ft_lstadd_back(cmd->exports, ft_lstnew((*tokens)->content));
		*tokens = (*tokens)->next;
	}
	while (!is_ctrl_op((*tokens)->content))
	{
		if (tokens->content->type == red_in)
			cmd->red_in = (*tokens)->next->content->str;
		else if ((*tokens)->content->type == red_out)
			cmd->red_out = (*tokens)->next->content->str;
		else if ((*tokens)->content->type == here_doc)
			cmd->hd_delimit = (*tokens)->next->content->str;
		else if ((*tokens)->content->type == word)
			ft_lstadd_back(cmd->args, ft_lstnew((*tokens)->content->str));
		else
			return (0);
		if ((*tokens)->content->type == red_in || (*tokens)->content->type == red_out
				|| (*tokens)->content->type == here_doc)
			*tokens = (*tokens)->next;
		*tokens = (*tokens)->next;
	}
	return (1);
}

void	parse_pipeline(t_list *tokens, t_list *cmds)
{
	while (tokens)
	{
	}
}

t_list	*parse_tokens(t_list	*tokens_head)
{
	t_list	*cmd_list;

	cmd_list = NULL;
	while (tokens)
	{
		ft_lstadd_back(cmd_list, new_cmd());
		parse_cmd(&tokens, cmd_list->content);
		if (token->type == pipe)
					
	}
	return (cmd_list);
}
