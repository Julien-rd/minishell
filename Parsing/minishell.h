#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include "libft_00/libft.h"

# define END 0
# define CMD 1
# define INFILE 2
# define OUTFILE 3
# define DEFAULT 4
# define OPERATOR 5

typedef struct s_input {
	char	**entries;
	char	**envp;
	int		*input_spec;
	size_t	total_entries;
	int		sgl_quote;
	int		dbl_quote;
}	t_input;

int		is_token(char c);
int		is_closed(char *str);
size_t	count_entries(char *buf, t_input *data);
int		malloc_ops(size_t *entry, size_t *iter, char *buf, t_input *data);
void 	fill_ops(size_t *entry, size_t *iter, char *buf, t_input *data);
int		malloc_entries(char *buf, t_input *data);
void	input_spec_init(t_input *data);
void	fill_string(char *buf, t_input *data);
int		parse_string(char *buf, t_input *data);
