/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 12:32:40 by eprottun          #+#    #+#             */
/*   Updated: 2025/09/30 18:14:41 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "structs.h"

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
# define INTERACTIVE 18
# define NONINTERACTIVE 19
# define HERE_DOC_OP 20
# define APPEND_OP 21
# define INFILE_OP 22
# define OUTFILE_OP 23
# define A_HERE_DOC 24
# define A_APPEND 25
# define A_INFILE 26
# define A_OUTFILE 27
# define NULL_DEFAULT 28

extern volatile int	g_current_signal;

void				entry_spec(t_input *data);
int					parse_and_execute(char *buf, t_input *data, int flag);
int					ultimate(char *buf, t_input *data);

// NONINTERACTIVE
void				non_interactive(t_input *data);

// SIGNALS
void				setup_heredoc_signals(void);
void				setup_main_signals(void);
void				setup_child_signals(void);
void				setup_interactive_signals(void);
void				setup_noninteractive_signals(void);
void				sigint_prompt(int num);
void				sigint_heredoc(int num);
void				sigint_main(int num);

int					malloc_ops(char *buf, size_t *entry, size_t *iter,
						t_input *data);
void				fill_ops(char *buf, size_t *entry, size_t *iter,
						t_input *data);
void				fill_string(t_input *data);

/* expand string */
int					expand_init(t_entry *cur, t_input *data,
						t_expanded_str *str);
int					quote_check(size_t iter, char *buf, t_input *data);
size_t				envlen(char *env);
size_t				envsize(char *env, char **envp, t_input *data);
char				*expand(t_entry *cur, t_input *data);
int					quoteclosed(char *str, char quote, t_input *data);
int					get_env(char *buf, t_expanded_str *str,
						t_expand_helper *exh, char **envp);
int					check_envs(char *buf, t_input *data, t_expanded_str *str);
char				*expanded_str(char *buf, t_input *data, t_expanded_str *str);
int					expand_entries(t_input *data);

/* parse_string */
size_t				count_entries(char *buf, t_input *data);
int					malloc_entries(char *buf, t_input *data);
void				input_spec_init(t_input *data);
void				fill_entries(char *buf, t_input *data);
int					parse_string(char *buf, t_input *data);
int					syntax_check(t_input *data);

/* parse_string_helpers */
int					token_len(char *buf, t_input *data, size_t iter);
int					is_token(char c);
int					is_closed(char *str);
int					toggle_quotes(char *str, t_input *data, size_t iter);

/* parse_string_ops */
void				op_count(char *buf, size_t *iter, size_t *count);
int					malloc_ops(char *buf, size_t *entry, size_t *iter,
						t_input *data);
void				fill_ops(char *buf, size_t *entry, size_t *iter,
						t_input *data);

int					ft_find_paths(char *envp[], char *env_name);
int					here_doc(t_exec *data);
char				*ft_getpath(char **envp, char *cmd);
int					exec_central(t_input *input);
int					execute_cmds(t_exec *data);
int					setup_redirect(t_exec *data, t_cmd *cmd);
int					check_cmd(t_exec *data, t_cmd *cmd);
int					cmd_flag(t_exec *data, t_cmd *cmd);
int					options_check(t_cmd *cmd);
void				internal_cmd_error(t_exec *data, t_cmd *cmd, int flag);
void				child_exit_handle(t_exec *data, t_cmd *cmd, int errcode);

/* own cmds */
void				pwd(t_exec *data, t_cmd *cmd, int flag);
int					exit_cmd(t_exec *data, t_cmd *cmd);
int					cd(t_exec *data, t_cmd *cmd, size_t pipe_count);
void				echo(t_exec *data, t_cmd *cmd, int nflag);
void				env(char **envp, t_exec *data, t_cmd *cmd, int flag);
int					insert_pos(t_exec *data, char *param);
int					export(char **cmd, t_exec *data);
int					unset(char **cmd, t_exec *data);
char				*ft_strjointhree(char const *s1, char const *s2,
						char const *s3);
// error messages

void				invalid_option(t_exec *data, t_cmd *cmd);
void				execve_fail(char *path, int error, t_exec *data,
						t_cmd *cmd);
void				command_fail(char *path, t_exec *data, t_cmd *cmd);
void				builtin_handler(t_exec *data, t_cmd *cmd);

// helper
int					safe_write(int fd, char *buf, size_t len);
void				free2d(char ***str);
long long			ft_atoll(const char *str);

#endif