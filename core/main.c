/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 09:48:56 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/09 12:30:21 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_current_signal = 0;

int	main(int argc, char *argv[], char *envp[])
{
	t_sh	sh;

	if (init_shell(&sh, argc, argv, envp) == -1)
		return (1);
	if (!isatty(STDIN_FILENO))
		non_interactive(&sh);
	else
		interactive_loop(&sh);
	return (0);
}
