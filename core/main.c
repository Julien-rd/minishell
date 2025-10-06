/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 09:48:56 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/06 16:28:10 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_current_signal = 0;

int	create_envp(t_sh *sh, char *envp[])
{
	size_t	iter;
	size_t	create_iter;

	iter = 0;
	while (envp[iter])
		iter++;
	sh->envp.count = iter;
	sh->envp.malloc = (iter + (iter == 0)) * 2;
	sh->envp.vars = malloc((sh->envp.malloc + 1) * sizeof(char *));
	if (!sh->envp.vars)
		return (perror("create_envp"), -1);
	create_iter = 0;
	while (create_iter < iter)
	{
		sh->envp.vars[create_iter] = ft_strdup(envp[create_iter]);
		if (!sh->envp.vars[create_iter])
			return (perror("create_envp"), free2d(&sh->envp.vars), -1);
		create_iter++;
	}
	sh->envp.vars[create_iter] = NULL;
	return (0);
}

void	check_exit_status(char *buf, int exit_code, t_sh *sh)
{
	if ((exit_code == -1 && g_current_signal == 0) || sh->exit)
	{
		if (sh->envp.vars)
			free2d(&sh->envp.vars);
		if (sh->exit || buf == NULL)
		{
			if (buf)
				free(buf);
			if (safe_write(1, "exit\n", 5) == -1)
				exit(1);
			exit(exit_code);
		}
		free(buf);
		exit(1);
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	t_sh	sh;
	char	*buf;
	int		exit_code;

	(void) argc;
	(void) argv;
	if (create_envp(&sh, envp) == -1)
		return (1);
	sh.exit_code = 0;
	if (!isatty(STDIN_FILENO))
		non_interactive(&sh);
	while (1)
	{
		setup_interactive_signals();
		buf = readline("minishell>> ");
		if (buf == NULL)
			break ;
		setup_main_signals();
		if (empty_prompt(buf))
			continue ;
		exit_code = parse_and_execute(buf, &sh, INTERACTIVE);
		check_exit_status(buf, exit_code, &sh);
	}
	rl_clear_history();
	return (free2d(&sh.envp.vars), safe_write(1, "exit\n", 5), sh.exit_code);
}
