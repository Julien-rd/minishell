#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include "libft_00/libft.h"

# define END 0
# define CMD 1
# define INFILE 2
# define OUTFILE 3

typedef struct s_input {
	char	**cmds;
	size_t	cmd_count;

	char	**file_names;
	size_t	file_count;

	char	**env_names;
	size_t	env_count;

	char	**envp;
	int		*input_spec;
	size_t	total_entries;
	int		sgl_quote;
	int		dbl_quote;
}	t_input;
