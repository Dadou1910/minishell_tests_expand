NAME            = Minishell

CC              = cc
CFLAGS          = -Wall -Wextra -Werror -g3
INCLUDES_DIR    = includes/
BUILTINS_DIR    = builtins/
PARSING_DIR		= parsing/
SRC_DIR         = exec/
PIPEX_DIR       = sources_bonus/
GNL_DIR         = get_next_line/
LIBFT_DIR       = Libft/
OBJ_DIR         = .obj/
INCLUDES_H      = -I./$(INCLUDES_DIR)
DEPS            = Makefile $(wildcard $(INCLUDES_DIR)*.h) $(wildcard $(GNL_DIR)*.h) $(wildcard $(PIPEX_DIR)*.h)

BUILTINS_SRC    = $(wildcard $(BUILTINS_DIR)*.c)
EXEC_SRC        = $(wildcard $(SRC_DIR)*.c)
PARSING_SRC		= $(wildcard $(PARSING_DIR)*.c)
PIPEX_SRC       = $(wildcard $(PIPEX_DIR)*.c)
GNL_SRC         = $(wildcard $(GNL_DIR)*.c)

SOURCES         = $(BUILTINS_SRC) $(EXEC_SRC) $(PIPEX_SRC) $(PARSING_SRC) $(GNL_SRC)

OBJ_FILES       = $(patsubst %.c,$(OBJ_DIR)%.o,$(SOURCES))

all: $(NAME)

$(OBJ_DIR)%.o: %.c $(DEPS)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES_H) -c -o $@ $<

$(NAME): $(OBJ_FILES)
	make -C $(LIBFT_DIR)
	@echo "\e[36mMaking $(NAME)...\e[0m"
	@$(CC) $(CFLAGS) $(INCLUDES_H) $(OBJ_FILES) -L$(LIBFT_DIR) -lreadline -lft -o $(NAME)
	@echo "\e[32mDone !\e[0m"

clean:
	@rm -rf $(OBJ_DIR)
	make clean -C $(LIBFT_DIR)
	@echo "\e[31mObject files removed.\e[0m"

fclean: clean
	@rm -f $(NAME)
	make fclean -C $(LIBFT_DIR)
	@echo "\e[31m$(NAME) removed.\e[0m"

re: fclean all

.PHONY: all clean fclean re
