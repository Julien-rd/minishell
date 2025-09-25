NAME = minishell
CC = cc
CFLAGS = -Ilibft_00 -MMD -g
SRC = exec/cmd_execution/own_cmds.c exec/cmd_execution/exit_status.c exec/cmd_execution/file_management.c exec/cmd_execution/internal_cmd_error.c exec/cmd_execution/check_cmd.c parse/read_line.c parse/signal_handling.c parse/parse_string/parse_string.c parse/parse_string/parse_string_ops.c parse/free2d.c parse/parse_string/parse_string_helpers.c parse/expand/expand_input.c parse/expand/expand_input_utils.c exec/here_doc.c exec/cmd_execution/execute_cmds.c exec/cmd_execution/find_path.c exec/exec_input.c
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)
LIBFT_DIR = libft_00
LIBFT = $(LIBFT_DIR)/libft.a

.SILENT:

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME) -lreadline 2>&1

clean: 
	rm -f $(OBJ) $(DEP)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

-include $(DEP)

.PHONY: all clean fclean re
