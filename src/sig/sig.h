/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 16:23:36 by joaoteix          #+#    #+#             */
/*   Updated: 2023/11/06 16:20:37 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIG_H
# define SIG_H

# include <libft.h>
# include <readline/readline.h>
# include <signal.h>
# include <unistd.h>

void	handle_interact_int(int sig);
void	bind_interact_sigs(void);
void	bind_exec_sigs(void);
#endif
