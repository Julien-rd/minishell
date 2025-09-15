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
	int		file_count;

	char	**env_names;
	int		env_count;
	int		*input_spec;
	int		tmp_spec;
}	t_input;
