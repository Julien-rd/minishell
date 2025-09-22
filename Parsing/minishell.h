/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 12:32:40 by eprottun          #+#    #+#             */
/*   Updated: 2025/09/22 13:16:40 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft_00/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

# define END 0
# define CMD 1
# define INFILE 2
# define OUTFILE 3
# define DEFAULT 4
# define OPERATOR 5
# define HERE_DOC 6
# define APPEND_FILE 7
# define PIPE 8

# define ECHO 9
# define CD 10
# define PWD 11
# define EXPORT 12
# define UNSET 13
# define EXIT 14
# define ENV 15
# define EXTERNAL 16
# define INTERNAL 17

typedef struct s_expanded_str
{
	size_t	var_count;
	char	**paths;
	size_t	*path_pos;
}			t_expanded_str;

typedef struct s_cmd
{
	char	**cmd;
	char	**line;
	int		*line_spec;
}			t_cmd;

typedef struct s_input
{
	char	**envp;
	size_t	envp_count;
	size_t	envp_malloc;
	char	*exp_str;
	char	**entries;
	int		*input_spec;
	size_t	total_entries;
	int		sgl_quote;
	int		dbl_quote;
	size_t	len;
}			t_input;

typedef struct s_exec
{
	char	**envp;
	size_t	envp_count;
	size_t	envp_malloc;

	char	**heredoc;
	size_t	hdoc_iter;

	char	**entries;
	int		*input_spec;

	int		*pipe_position;
	size_t	pipe_iter;
	size_t	pipe_count;

	int		cmd_flag;
	int		internal_errcode;
	pid_t	pid;
	pid_t	last_pid;
	int		prev_fd;
	int		fd[2];
}			t_exec;

size_t		pathlen(char *path);
size_t		pathsize(char *path, char **envp, t_input *data);
int			expand_input(char *buf, char **envp, t_input *data);
int			quoteclosed(char *str, char quote, t_input *data);
int			getpath(char *buf, t_expanded_str *str, size_t path_iter,
				char **envp, size_t len);
int			quote_check(size_t iter, char *buf, t_input *data);
void		free2d(char **str);

size_t		count_entries(t_input *data);
int			malloc_ops(size_t *entry, size_t *iter, t_input *data);
void		fill_ops(size_t *entry, size_t *iter, t_input *data);
void		fill_string(t_input *data);

/* parse_string */
size_t		count_entries(t_input *data);
int			malloc_entries(t_input *data);
void		input_spec_init(t_input *data);
void		fill_entries(t_input *data);
int			parse_string(t_input *data);

/* parse_string_helpers */
int			token_len(t_input *data, size_t *iter);
int			is_token(char c);
int			is_closed(char *str);
int			toggle_quotes(t_input *data, size_t iter);

/* parse_string_ops */
void		op_count(char *buf, size_t *iter, size_t *count);
int			malloc_ops(size_t *entry, size_t *iter, t_input *data);
void		fill_ops(size_t *entry, size_t *iter, t_input *data);

int			ft_find_paths(char *envp[], char *env_name);
int			here_doc(t_exec *data);
char		*ft_getpath(char **envp, char *cmd);
int			exec_central(t_input *input, char **envp);
int			execute_cmds(t_exec *data);
int			setup_redirect(t_exec *data, t_cmd *cmd);
int			check_cmd(t_exec *data, t_cmd *cmd);
int	cmd_flag(t_exec *data, t_cmd *cmd);
static int	flag_check(t_cmd *cmd);

/* own cmds */
int			pwd(void);
int			own_exit(int exit_code);
int			cd(char *path);
int			echo(char **cmd, int nflag);
int			env(char **envp);
int			export(char **cmd, t_exec *data);
int			unset(char **cmd, t_exec *data);

#endif