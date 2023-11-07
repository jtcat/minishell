/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 16:23:36 by joaoteix          #+#    #+#             */
/*   Updated: 2023/11/06 23:27:10 by jcat             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIG_H
# define SIG_H

# include <libft.h>
# include <stdio.h>
# include <readline/readline.h>
# include <signal.h>
# include <unistd.h>

void	handle_interact_int(int sig);
void	bind_interact_sigs(void);
void	bind_exec_sigs(void);
#endif
