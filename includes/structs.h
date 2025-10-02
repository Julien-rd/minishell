/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 11:42:04 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/02 11:35:40 by eprottun         ###   ########.fr       */
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

typedef struct s_cmd
{
	char			**cmd;
	int				cmd_flag;
	t_entry			*line;
}					t_cmd;

typedef struct s_envp
{
	char			**vars;
	size_t			count;
	size_t			malloc;
}					t_envp;

// typedef struct s_input
// {
// 	t_envp			envp;
// 	char			**heredoc;
// 	t_entry			*entries;
// 	int				exit_code;
// 	int				exit;
// }					t_sh;

typedef struct s_pipe
{
	int				*position;
	size_t			iter;
	size_t			count;
	size_t			hdoc_iter;
	pid_t			pid;
	pid_t			last_pid;
	int				prev_fd;
	int				fd[2];	
}					t_pipe;

typedef struct s_sh
{
	t_envp			envp;
	t_entry			*entries;
	t_pipe			pipe;
	char			**heredoc;
	int				internal_errcode;
	int				exit;
	int				exit_code;
	int				sgl_quote;
	int				dbl_quote;
}					t_sh;

#endif