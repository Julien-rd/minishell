/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 12:32:40 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/07 17:37:42 by eprottun         ###   ########.fr       */
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
# define NUMERIC_ARG 29
# define TOO_MANY_ARGS 30

extern volatile sig_atomic_t	g_current_signal;

/********************  CORE  ********************/

/******  Signals  ******/
void							setup_heredoc_signals(void);
void							setup_main_signals(void);
void							setup_child_signals(void);
void							setup_interactive_signals(void);
void							setup_noninteractive_signals(void);

void							sigint_prompt(int num);
void							sigint_heredoc(int num);
void							sigint_main(int num);
int								hdoc_signal_kill(char *buf, char *entry);

/******  Helpers  ******/
int								safe_write(int fd, char *buf, size_t len);
void							free2d(char ***str);
long long						ft_atoll(const char *str);
char							*remove_quotes(char *to_strip, size_t len);
size_t							skip_whitspaces(char *buf);
int								toggle_quotes(char *str, t_sh *sh, size_t iter);
int								is_token(char c);
void							cut_nl(char *buf);
bool							empty_prompt(char *buf);
void							free_list(t_entry *list);
void							child_exit_handle(t_sh *sh, t_pipeline *pl,
									int errcode);

/********************  PARSING  ********************/

int								parsing(char *buf, t_sh *sh);

// BUILD_HELPERS
t_entry							*lstlast(t_entry *lst);
void							lstadd(t_entry **lst, t_entry *new);
t_entry							*newnode(char *raw_str);
int								parse_and_execute(char *buf, t_sh *sh,
									int flag);

// NONINTERACTIVE
void							non_interactive(t_sh *sh);

int								malloc_ops(char *buf, size_t *entry,
									size_t *iter, t_sh *sh);
void							fill_ops(char *buf, size_t *entry, size_t *iter,
									t_sh *sh);
void							fill_string(t_sh *sh);

/********************  EXECUTION  ********************/

/******  Builtins  ******/
void							pwd(t_sh *sh, t_pipeline *pl, int flag);
int								exit_cmd(t_pipeline *pl, t_sh *sh);
int								cd(t_sh *sh, char **argv, size_t pipe_count);
void							echo(t_pipeline *pl, t_sh *sh, int nflag);
void							env(t_pipeline *pl, t_sh *sh, int flag);
int								export(char **argv, t_pipeline *pl, t_sh *sh);
int								unset(char **cmd, t_pipeline *pl, t_sh *sh);

void							free_cmds(t_pipeline *pl, size_t arr_len);

/******************** EXPAND ********************/
int								expand_init(t_entry *current, t_sh *sh,
									t_expand_str *str);
int								quote_check(size_t iter, char *buf, t_sh *sh);
size_t							envlen(char *env);
char							*expand(t_entry *current, t_sh *sh, int flag);
int								quoteclosed(char *str, char quote, t_sh *sh);
int								get_env(char *buf, t_expand_str *str,
									t_expand_helper *exh, char **envp);
int								check_envs(char *buf, t_sh *sh,
									t_expand_str *str);
char							*expanded_str(char *buf, t_sh *sh,
									t_expand_str *str);
int								split_expands(char *exp_str, t_entry *entry,
									t_sh *sh);
int								ex_encounter(char *str_new,
									t_expand_helper *exh, t_expand_str *str,
									size_t iter);
int								check_return_get_env(size_t iter,
									t_expand_str *str, t_expand_helper *exh);

/* parse_string */
size_t							count_entries(char *buf, t_sh *sh);
int								malloc_entries(char *buf, t_sh *sh);
void							input_spec_init(t_sh *sh);
void							fill_entries(char *buf, t_sh *sh);
int								parse_string(char *buf, t_sh *sh);
int								syntax_check(t_sh *sh);

/* parse_string_helpers */
int								token_len(char *buf, t_sh *sh, size_t iter);
int								is_closed(char *str);

/* parse_string_ops */
void							op_count(char *buf, size_t *iter,
									size_t *count);
int								malloc_ops(char *buf, size_t *entry,
									size_t *iter, t_sh *sh);
void							fill_ops(char *buf, size_t *entry, size_t *iter,
									t_sh *sh);

int								ft_find_paths(char *envp[], char *env_name);
int								here_doc(t_sh *sh);
char							*ft_getpath(char **envp, char *cmd);
int								exec_central(t_sh *input);
int								pipeline(t_sh *sh);
int								pipe_init(t_pipeline *pl);
int								setup_redirect(t_sh *sh, t_pipeline *pl);
// int					check_cmd(t_sh *sh, t_cmd*cmd);
void							cmd_flag(t_sh *sh, t_cmd *current);
int								options_check(t_cmd *cmd);
void							internal_cmd_error(t_pipeline *pl, t_sh *sh,
									int flag);

/* own cmds */
char							*ft_strjointhree(char const *s1, char const *s2,
									char const *s3);
int								insert_pos(t_sh *sh, char *param);
// error messages

void							invalid_option(t_pipeline *pl, t_sh *sh);
void							execve_fail(char *path, int error,
									t_pipeline *pl, t_sh *sh);
void							command_fail(t_pipeline *pl,
									t_sh *sh);
void							builtin_handler(t_pipeline *pl, t_sh *sh);
int								syntax_error(t_entry *entry);
int								input_check(char *param);

// helper

int								count_pipes(t_sh *sh);
int								init_pipe_pos(t_pipeline *pl, t_sh *sh);
void							find_start(t_pipeline *pl, t_sh *sh,
									size_t cmd_iter);
int								cmd_init(t_cmd *current);
int								pipe_fork(t_pipeline *pl);

int								setup_cmds(t_pipeline *pl, t_sh *sh);
int								own_cmd_exec(t_pipeline *pl, t_sh *sh);
void							child_process(t_pipeline *pl, t_sh *sh);
int								parent_process(t_pipeline *pl);
int								kill_children(t_pipeline *pl, t_sh *sh);

#endif