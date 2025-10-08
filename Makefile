NAME = minishell
CC = cc
INC_DIR = includes
CFLAGS = -Wall -Wextra -Werror -Ilibft -MMD -g -I$(INC_DIR)
OBJ_DIR = obj
VPATH = parsing parsing/expand exec exec/builtins exec/pipeline core core/signals core/non_interactive/get_next_line parsing/expand parsing/here_doc core/non_interactive \
exec/error_handling exec/pipeline_helpers core/helper
SRC = split_expands.c syntax_check.c parsing.c build_helpers.c helper.c error_messages.c file_management.c internal_cmd_error.c \
check_cmd.c main.c signal_handlers.c free_functions.c\
expand.c expand_helper.c expand_helper2.c here_doc.c \
pipeline.c find_path.c exec_central.c get_next_line.c \
get_next_line_utils.c non_interactive.c signal_setups.c parse_and_execute.c pipe_init.c \
cd.c echo.c env.c exit.c export.c pwd.c unset.c pl_functions.c pl_helper.c non_interactive_helpers.c builtin_handler.c


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
