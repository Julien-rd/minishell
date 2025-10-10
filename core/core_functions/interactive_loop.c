/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interactive_loop.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 12:22:37 by jromann           #+#    #+#             */
/*   Updated: 2025/10/10 10:08:50 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	interactive_loop(t_sh *sh)
{
	while (1)
	{
		setup_interactive_signals();
		sh->buf = readline("minishell>> ");
		if (sh->buf == NULL)
			break ;
		setup_main_signals(sh);
		if (empty_prompt(sh->buf))
			continue ;
		parse_and_execute(sh);
		add_history(sh->buf);
		check_exit_status(sh);
	}
	rl_clear_history();
	check_exit_status(sh);
}
