#include "libft_00/libft.h"
#include <errno.h>
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define END 0
#define CMD 1
#define INFILE 2
#define OUTFILE 3
#define DEFAULT 4
#define OPERATOR 5
#define HERE_DOC 6
#define APPEND_FILE 7
#define PIPE 10

typedef struct s_cmd
{
	char	**cmd;
	char	**line;
	int		*line_spec;
}			t_cmd;

typedef struct s_input
{
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

	char	**heredoc;
	size_t	hdoc_iter;

	char	**entries;
	int		*input_spec;

	int		*pipe_position;
	size_t	pipe_iter;
	size_t	pipe_count;

	pid_t	pid;
	pid_t	last_pid;
	int		prev_fd;
	int		fd[2];
	int		IO[2];
	int		return_value;
}			t_exec;

int			quoteclosed(char *str, char quote, t_input *data);
size_t		pathlen(char *path);
size_t		pathsize(char *path, char **envp, t_input *data);
char		*getpath(char *path, char **envp, t_input *data);
void		expand_input(char *buf, char **envp, t_input *data);

size_t		count_entries(t_input *data);
int			malloc_ops(size_t *entry, size_t *iter, t_input *data);
void		fill_ops(size_t *entry, size_t *iter, t_input *data);
void		fill_string(t_input *data);

/* parse_string */
int			is_token(char c);
int			is_closed(char *str);
void		op_count(char *buf, size_t *iter, size_t *count);
int			toggle_quotes(t_input *data, size_t iter);
size_t		count_entries(t_input *data);
int			malloc_ops(size_t *entry, size_t *iter, t_input *data);
void		fill_ops(size_t *entry, size_t *iter, t_input *data);
int			token_len(t_input *data, size_t *iter);
int			malloc_entries(t_input *data);
void		input_spec_init(t_input *data);
void		fill_entries(t_input *data);
int			parse_string(t_input *data);

int			here_doc(t_exec *data);
char		*ft_getpath(char **envp, char *cmd);
int			exec_central(t_input *input, char **envp);
int			execute_cmds(t_exec *data);
