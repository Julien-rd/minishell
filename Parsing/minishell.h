#include "libft_00/libft.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

#define END 0
#define CMD 1
#define INFILE 2
#define OUTFILE 3
#define DEFAULT 4
#define OPERATOR 5

typedef struct s_input
{
	char	*token_str;
	char	**entries;
	char	**envp;
	int		*input_spec;
	size_t	total_entries;
	int		sgl_quote;
	int		dbl_quote;
	size_t	len;
}			t_input;

// PARSE
int			is_token(char c);
int			is_closed(char *str);
size_t		count_entries(char *buf, t_input *data);
int			malloc_ops(size_t *entry, size_t *iter, char *buf, t_input *data);
void		fill_ops(size_t *entry, size_t *iter, char *buf, t_input *data);
int			malloc_entries(char *buf, t_input *data);
void		input_spec_init(t_input *data);
void		fill_string(char *buf, t_input *data);
int			parse_string(char *buf, t_input *data);
char		*getpath(char *path, char **envp, t_input *data);
size_t		pathsize(char *path, char **envp, t_input *data);
size_t		pathlen(char *path);
int			quoteclosed(char *str, char quote, t_input *data);
void		expand_input(char *buf, char **envp, t_input *data);

// EXEC
char		*heredoc(t_input *data, int heredoc_pos);
int			ft_strcmp(const char *s1, const char *s2);
