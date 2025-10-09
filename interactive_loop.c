/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interactive_loop.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 12:22:37 by jromann           #+#    #+#             */
/*   Updated: 2025/10/09 12:54:10 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void interactive_loop(t_sh	*sh)
{
	char	*buf;
	int		exit_code;
	
	while (1)
	{
		setup_interactive_signals();
		buf = readline("minishell>> ");
		if (buf == NULL)
			break ;
		setup_main_signals();
		if (empty_prompt(buf))
			continue ;
		exit_code = parse_and_execute(buf, sh);
		add_history(buf);
		check_exit_status(buf, exit_code, sh);
	}
	rl_clear_history();
    check_exit_status(buf, sh->exit_code, sh);
}