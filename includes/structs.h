/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 11:42:04 by eprottun          #+#    #+#             */
/*   Updated: 2025/09/27 11:48:24 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef STRUCTS_H
# define STRUCTS_H

# include "libraries.h"

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
	char			**line;
	int				*line_spec;
}					t_cmd;

typedef struct s_input
{
	char			**envp;
	size_t			envp_count;
	size_t			envp_malloc;
	char			*exp_str;
	char			**entries;
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