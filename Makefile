NAME = minishell
CC = cc
INC_DIR = includes
CFLAGS = -Wall -Wextra -Werror -Ilibft -MMD -g -I$(INC_DIR)
OBJ_DIR = obj
VPATH = core core/core_functions core/core_functions/non_interactive exec core/core_functions/non_interactive/get_next_line exec/builtins exec/pipeline/pipeline_functions/file_handling exec/error_handling exec/pipeline exec/pipeline/pipeline_functions helper parsing parsing/parsing_functions parsing/parsing_functions/expand signals
SRC = split_expands.c syntax_check.c parsing.c list_helpers.c helper.c error_messages.c file_handler.c internal_cmd_error.c \
 main.c signal_handlers.c free_functions.c \
expand.c expand_helper.c expand_helper2.c here_doc.c interactive_loop.c \
pipeline.c find_path.c get_next_line.c check_exit_status.c init_shell.c\
get_next_line_utils.c non_interactive.c signal_setups.c parse_and_execute.c \
cd.c echo.c env.c exit.c export.c pwd.c unset.c builtin_handler.c pipe_init.c pipeline_core.c pipeline_helpers.c \
setup.c


OBJ = $(SRC:%.c=obj/%.o)
DEP = $(SRC:%.c=obj/%.d)
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

.SILENT:

all: $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -MMD -MF $(@:.o=.d) -c $< -o $@

$(NAME): $(LIBFT) $(OBJ_DIR) $(OBJ) 
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME) -lreadline 2>&1

clean: 
	rm -f $(OBJ) $(DEP)
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

-include $(DEP)

.PHONY: all clean fclean re
