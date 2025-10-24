/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 13:58:47 by jromann           #+#    #+#             */
/*   Updated: 2025/10/21 12:26:41 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void  print_declare_x(t_pipeline *pl, t_sh *sh)
{
   size_t  iter;


   iter = 0;
   if (pl->current->argv[1])
	return ;
   while (sh->envp.vars[iter])
   {
       if (safe_write(1, "declare -x " , 12))
           child_exit(sh, pl, 1);
       if (safe_write(1, sh->envp.vars[iter], ft_strlen(sh->envp.vars[iter])) == -1)
           child_exit(sh, pl, 1);
       if (safe_write(1, "\n" , 1))
           child_exit(sh, pl, 1);
       iter++;
   }
   child_exit(sh, pl, 0);
}

void	builtin_handler(t_pipeline *pl, t_sh *sh)
{
	int	flag;

	flag = options_check(pl->current);
	if (pl->current->cmd_flag == EXPORT)
		print_declare_x(pl, sh);
	if (pl->current->cmd_flag == ECHO)
		echo(pl, sh, flag);
	else if (pl->current->cmd_flag == PWD)
		pwd(sh, pl, flag);
	else if (pl->current->cmd_flag == ENV)
		env(pl, sh, flag);
	else
		internal_cmd_error(pl, sh, flag);
}
