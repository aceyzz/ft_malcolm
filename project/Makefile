# ---------------------------------------------------------------------------- #
# 								Variables									   #
# ---------------------------------------------------------------------------- #
NAME        := ft_malcolm

SRCDIR      := sources
OBJDIR      := .obj
INCDIR      := includes

LIBFT_DIR   := $(INCDIR)/libft
LIBFT_A     := $(LIBFT_DIR)/libft.a

CC          := gcc
CFLAGS      := -Wall -Wextra -Werror -g
CPPFLAGS    := -I$(INCDIR) -I$(LIBFT_DIR)
RM          := rm -rf
MAKEFLAGS   := --no-print-directory
SRC         := $(shell find $(SRCDIR) -name '*.c')
OBJ         := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))

CYAN		:= \033[0;36m
GREEN		:= \033[0;32m
YELLOW		:= \033[0;33m
RED			:= \033[0;31m
NC			:= \033[0m

# ---------------------------------------------------------------------------- #
# 									Regles								       #
# ---------------------------------------------------------------------------- #
all: $(LIBFT_A) $(NAME)

$(NAME): $(OBJ) $(LIBFT_A)
	@$(CC) $(CFLAGS) $(OBJ) -L$(LIBFT_DIR) -lft -o $@
	@echo "$(NAME) créé."

$(LIBFT_A):
	@$(MAKE) -C $(LIBFT_DIR) > /dev/null 2>&1 || true

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
	@echo "Compilation de $<"

clean:
	@$(RM) $(OBJDIR) > /dev/null 2>&1 || true
	@$(MAKE) -C $(LIBFT_DIR) clean > /dev/null 2>&1 || true
	@echo "Objets nettoyés."

fclean: clean
	@$(RM) $(NAME) > /dev/null 2>&1 || true
	@$(MAKE) -C $(LIBFT_DIR) fclean > /dev/null 2>&1 || true
	@echo "Binaire et libft.a supprimés."

re: fclean all

leaks: all
	sudo -E valgrind --leak-check=full --show-leak-kinds=all ./ft_malcolm 192.168.64.3 00:11:22:33:44:55 192.168.64.5 a2:4c:49:b4:fd:0e

.PHONY: all clean fclean re leaks
