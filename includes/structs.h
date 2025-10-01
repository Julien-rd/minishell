/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 11:42:04 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/01 13:28:54 by jromann          ###   ########.fr       */
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

typedef struct s_expanded_str
{
	size_t			var_count;
	char			**env_arr;
	size_t			*env_pos;
	int				exit_code;
}					t_expanded_str;

typedef struct s_cmd
{
	char			**cmd;
	t_entry			*line;
	// char			**line;
	int				*line_spec;
}					t_cmd;

typedef struct s_input
{
	char			**envp;
	char			**heredoc;
	size_t			envp_count;
	size_t			envp_malloc;
	char			*exp_str;
	t_entry			*entries;
	int				*input_spec;
	size_t			total_entries;
	int				sgl_quote;
	int				dbl_quote;
	size_t			len;
	int				exp_str_malloc;
	int				exit_code;
	int				exit;
}					t_input;

typedef struct s_exec
{
	char			**envp;
	size_t			envp_count;
	size_t			envp_malloc;

	char			**heredoc;
	size_t			hdoc_iter;
	t_entry			*list;
	char			**entries;
	int				*input_spec;

	int				*pipe_position;
	size_t			pipe_iter;
	size_t			pipe_count;

	int				cmd_flag;
	int				internal_errcode;
	pid_t			pid;
	pid_t			last_pid;
	int				prev_fd;
	int				fd[2];
	int				exit;
	int				exit_code;
}					t_exec;

#endif