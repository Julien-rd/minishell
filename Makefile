NAME = minishell
CC = cc
INC_DIR = includes
CFLAGS = -Ilibft_00 -MMD -g -I$(INC_DIR)
OBJ_DIR = obj
VPATH = exec/cmd_execution parse parse/parse_string parse/expand exec
SRC = helper.c own_cmds.c error_messages.c file_management.c internal_cmd_error.c \
check_cmd.c read_line.c signal_handling.c parse_string.c parse_string_ops.c \
parse_string_helpers.c expand_input.c expand_input_utils.c here_doc.c \
execute_cmds.c find_path.c exec_input.c
OBJ = $(SRC:%.c=obj/%.o)
DEP = $(SRC:%.c=obj/%.d)
LIBFT_DIR = libft_00
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
	rm -d $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

-include $(DEP)

.PHONY: all clean fclean re
