/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 09:48:56 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/02 11:29:22 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile int	g_current_signal = 0;

// void	entry_spec(t_sh *sh)
// {
// 	size_t	iter;

// 	iter = 0;
// 	while (sh->entries[iter])
// 	{
// 		if (sh->input_spec[iter] == OPERATOR)
// 		{
// 			if (!ft_strcmp(sh->entries[iter], "<<"))
// 			{
// 				sh->input_spec[iter] = HERE_DOC_OP;
// 				if (sh->input_spec[iter + 1] == DEFAULT)
// 					sh->input_spec[iter + 1] = HERE_DOC;
// 			}
// 			else if (!ft_strcmp(sh->entries[iter], ">>"))
// 			{
// 				sh->input_spec[iter] = APPEND_OP;
// 				if (sh->input_spec[iter + 1] == DEFAULT)
// 					sh->input_spec[iter + 1] = APPEND_FILE;
// 			}
// 			else if (!ft_strcmp(sh->entries[iter], "<"))
// 			{
// 				sh->input_spec[iter] = INFILE_OP;
// 				if (sh->input_spec[iter + 1] == DEFAULT)
// 					sh->input_spec[iter + 1] = INFILE;
// 			}
// 			else if (!ft_strcmp(sh->entries[iter], ">"))
// 			{
// 				sh->input_spec[iter] = OUTFILE_OP;
// 				if (sh->input_spec[iter + 1] == DEFAULT)
// 					sh->input_spec[iter + 1] = OUTFILE;
// 			}
// 			else if (!ft_strcmp(sh->entries[iter], "|"))
// 				sh->input_spec[iter] = PIPE;
// 		}
// 		iter++;
// 	}
// }

int	create_envp(t_sh *sh, char *envp[])
{
	size_t	iter;
	size_t	create_iter;

	if (!envp)
		return (write(1, "create_envp: envp not found\n", 29), -1);
	iter = 0;
	while (envp[iter])
		iter++;
	sh->envp.count = iter;
	sh->envp.malloc = iter * 2;
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

int	main(int argc, char *argv[], char *envp[])
{
	t_sh	sh;
	char	*buf;
	int		exit_code;

	if (create_envp(&sh, envp) == -1)
		return (1);
	sh.exit_code = 0;
	sh.exit = 0;
	if (!isatty(STDIN_FILENO))
		non_interactive(&sh);
	while (1)
	{
		g_current_signal = 0;
		setup_interactive_signals();
		buf = readline("minishell>> ");
		if (buf == NULL)
			break ;
		setup_main_signals();
		if (empty_prompt(buf))
		{
			free(buf);
			continue ;
		}
		exit_code = parse_and_execute(buf, &sh, INTERACTIVE);
		if (exit_code == -1 && g_current_signal == 0)
			return (free(buf), free2d(&sh.envp.vars), 1);
		if (sh.exit)
			return (free(buf), safe_write(1, "exit\n", 5), free2d(&sh.envp.vars),
				exit_code);
		free(buf);
	}
	rl_clear_history();
	return (free2d(&sh.envp.vars), safe_write(1, "exit\n", 5), 0);
}
