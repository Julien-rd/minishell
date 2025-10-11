/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 11:42:04 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/10 10:52:47 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include "libraries.h"

typedef struct s_entry
{
	char			*raw_entry;
	int				spec;
	char			**expanded;
	int				exp_count;
	struct s_entry	*next;
}					t_entry;

typedef struct s_expand_helper
{
	size_t			env_iter;
	size_t			env_pos_iter;
	size_t			len;
	size_t			str_iter;
	int env_return ;
	char			*buf;
}					t_expand_helper;

typedef struct s_expand_str
{
	size_t			var_count;
	char			**env_arr;
	size_t			*env_pos;
	int				exit_code;
	int				flag;
	int				len;
}					t_expand_str;

typedef struct s_envp
{
	char			**vars;
	size_t			count;
	size_t			malloc;
}					t_envp;

typedef struct s_cmd
{
	char			**argv;
	int				cmd_flag;
	t_entry			*line;
	pid_t			pid;
}					t_cmd;

typedef struct s_pipeline
{
	t_cmd			*cmds;
	t_cmd			*current;
	int				*position;
	size_t			iter;
	size_t			count;
	size_t			hdoc_iter;
	pid_t			last_pid;
	int				prev_fd;
	int				fd[2];
}					t_pipeline;

typedef struct s_sh
{
	t_envp			envp;
	t_entry			*entries;
	char			**heredoc;
	int				internal_errcode;
	int				exit;
	int				exit_code;
	int				sgl_quote;
	int				dbl_quote;
}					t_sh;

#endif