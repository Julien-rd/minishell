/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 12:32:40 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/10 10:09:00 by jromann          ###   ########.fr       */
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
# define INTERACTIVE 18
# define NONINTERACTIVE 19
# define HERE_DOC_OP 20
# define APPEND_OP 21
# define INFILE_OP 22
# define OUTFILE_OP 23
# define NULL_DEFAULT 28
# define NUMERIC_ARG 29
# define TOO_MANY_ARGS 30
# define FAILURE 31
# define SUCCESS 32

extern volatile sig_atomic_t	g_current_signal;

/********************** 1. CORE  ************************/

/******  Core Functions  ******/

int								init_shell(t_sh *sh, int argc, char **argv,
									char **envp);
void							non_interactive(t_sh *sh);
void							interactive_loop(t_sh *sh);
void							parse_and_execute(t_sh *sh);
void							check_exit_status(t_sh *sh);

/******************** 2. EXECUTION **********************/

/********  Builtins  ********/

void							builtin_handler(t_pipeline *pl, t_sh *sh);
void							pwd(t_sh *sh, t_pipeline *pl, int flag);
int								exit_cmd(t_pipeline *pl, t_sh *sh);
int								cd(t_sh *sh, char **argv, size_t pipe_count);
void							echo(t_pipeline *pl, t_sh *sh, int nflag);
void							env(t_pipeline *pl, t_sh *sh, int flag);
int								input_check(char *param);
int								export(char **argv, t_pipeline *pl, t_sh *sh);
int								unset(char **cmd, t_pipeline *pl, t_sh *sh);

/********  Pipeline  ********/

int								pipeline(t_sh *sh);

// pipeline_core
int								pl_setup(t_pipeline *pl, t_sh *sh);
int								own_cmd_exec(t_pipeline *pl, t_sh *sh);
void							child_process(t_pipeline *pl, t_sh *sh);
int								parent_process(t_pipeline *pl);
int								kill_children(t_pipeline *pl, t_sh *sh);

// setup
int								count_pipes(t_sh *sh);
int								init_pipe_pos(t_pipeline *pl, t_sh *sh);
void							find_start(t_pipeline *pl, t_sh *sh,
									size_t cmd_iter);
int								cmd_init(t_cmd *current);
void							cmd_flag(t_sh *sh, t_cmd *current);

// find_path
int								ft_find_paths(char *envp[], char *env_name);
char							*ft_strjointhree(char const *s1, char const *s2,
									char const *s3);
char							*ft_getpath(char **envp, char *cmd);

// pipeline_helpers
int								options_check(t_cmd *cur);
int								cmd_tokens(t_cmd *current);
int								pipe_fork(t_pipeline *pl);
int								pl_cleanup(t_pipeline *pl, t_sh *sh, int flag);

// file_handling
int								setup_redirect(t_sh *sh, t_pipeline *pl);
int								pipe_init(t_pipeline *pl);

/******  Error Handling  ******/

// error_messages
long long						ft_atoll(const char *str);
void							invalid_option(t_pipeline *pl, t_sh *sh);
void							execve_fail(char *path, int error,
									t_pipeline *pl, t_sh *sh);
void							command_fail(t_pipeline *pl, t_sh *sh);

// internal_cmd_error
void							internal_cmd_error(t_pipeline *pl, t_sh *sh,
									int flag);

/********************** 3. HELPER ***********************/

// free_functions
void							free2d(char ***str);
void							free_list(t_entry *list);
void							child_exit_handle(t_sh *sh, t_pipeline *pl,
									int errcode);
void							free_cmds(t_pipeline *pl, size_t arr_len);
void							cleanup(t_sh *sh);

// helper
int								safe_write(int fd, char *buf, size_t len);
int								is_token(char c);
int								toggle_quotes(char *buf, t_sh *sh, size_t iter);
size_t							skip_whitspaces(char *buf);
bool							empty_prompt(char *buf);

/********************** 4. PARSING **********************/

int								parsing(t_sh *sh);

/********** Parsing Functions **********/

int								syntax_check(t_sh *sh);
int								here_doc(t_sh *sh);

// list_helpers
t_entry							*lstlast(t_entry *lst);
void							lstadd(t_entry **lst, t_entry *new);
t_entry							*newnode(char *raw_str);

/*************** Expand ***************/

char							*expand(t_entry *current, t_sh *sh, int flag);

// expand_helper
char							*remove_quotes(char *to_strip, size_t len);
int								check_return_get_env(size_t iter,
									t_expand_str *str, t_expand_helper *exh);
int								ex_encounter(char *str_new,
									t_expand_helper *exh, t_expand_str *str,
									size_t iter);

// expand_helper2
int								get_env(char *buf, t_expand_str *str,
									t_expand_helper *exh, char **envp);
int								quote_check(size_t iter, char *buf, t_sh *sh);
size_t							envlen(char *env);
int								get_env(char *buf, t_expand_str *str,
									t_expand_helper *exh, char **envp);

// split_expands
int								split_expands(char *exp_str, t_entry *entry,
									t_sh *sh);
int								token_len(char *buf, t_sh *sh, size_t iter);

/********************** 5. SIGNALS **********************/

// signal_handlers
void							sigint_prompt(int num);
void							sigint_main(int num);
void							sigint_heredoc(int num);
int								hdoc_signal_kill(char *buf, char *entry);

// signal_setups
void							setup_interactive_signals(void);
void							setup_main_signals(t_sh *sh);
void							setup_heredoc_signals(void);
void							setup_child_signals(void);
void							setup_noninteractive_signals(void);

#endif