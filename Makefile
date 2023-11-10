CC:= cc

CFLAGS:= -O0 -g -Wall -Wextra -Werror
#CFLAGS:= -fsanitize=address -O0 -g -Wall -Wextra -Werror

LIB_FLAGS:= -lreadline

NAME:= minishell

LFT_DIR:= src/libft
LFT:= $(LFT_DIR)/libft.a

SRCS:= $(addprefix src/, \
	main.c \
	$(addprefix lexer/, lexer.c) \
	$(addprefix utils/, str_utils.c) \
	$(addprefix parser/, grammar.c grammar2.c parser_utils.c hdoc.c) \
	$(addprefix exec/, exec.c exec2.c exec3.c expan.c redir.c) \
	$(addprefix env/, env.c env2.c) \
	$(addprefix builtins/, builtins.c builtins2.c builtins3.c) \
	$(addprefix dl_list/, ft_dlstadd_back.c ft_dlstclear.c ft_dlstdelone.c ft_dlstlast.c ft_dlstnew.c ft_dlstrmone.c ft_dlstdup.c) \
	$(addprefix sig/, sig.c) \
	$(addprefix gnl/, gnl.c) \
)

SRCS_BONUS:= $(addprefix src/, \
	main_bonus.c \
	$(addprefix lexer/, lexer_bonus.c) \
	$(addprefix utils/, str_utils_bonus.c) \
	$(addprefix parser/, grammar_bonus.c grammar2_bonus.c parser_utils_bonus.c hdoc_bonus.c) \
	$(addprefix exec/, exec_bonus.c exec2_bonus.c exec3_bonus.c expan_bonus.c redir_bonus.c) \
	$(addprefix env/, env_bonus.c env2_bonus.c) \
	$(addprefix builtins/, builtins_bonus.c builtins2_bonus.c builtins3_bonus.c) \
	$(addprefix dl_list/, ft_dlstadd_back_bonus.c ft_dlstclear_bonus.c ft_dlstdelone_bonus.c ft_dlstlast_bonus.c ft_dlstnew_bonus.c ft_dlstrmone_bonus.c ft_dlstdup_bonus.c) \
	$(addprefix sig/, sig_bonus.c) \
	$(addprefix gnl/, gnl_bonus.c) \
)

INC:= $(addprefix -I, src/ src/exec src/env src/libft src/parser src/lexer src/utils src/dl_list src/sig src/gnl)
 
HDR:= $(addprefix src/, minishell.h exec/exec.h env/env.h parser/parser.h lexer/lexer.h utils/shell_utils.h dl_list/dl_list.h sig/sig.h gnl/gnl.h builtins/builtins.h)

HDR_BONUS:= $(addprefix src/, minishell_bonus.h exec/exec_bonus.h env/env_bonus.h parser/parser_bonus.h lexer/lexer_bonus.h utils/shell_utils_bonus.h dl_list/dl_list_bonus.h sig/sig_bonus.h gnl/gnl_bonus.h builtins/builtins_bonus.h)

OBJS:= $(SRCS:.c=.o)

OBJS_BONUS:= $(SRCS_BONUS:.c=.o)

all: $(NAME)

bonus: $(HDR_BONUS) $(LFT) $(OBJS_BONUS)
	$(NAME)

$(NAME): $(HDR) $(LFT) $(OBJS)
	$(CC) $(CFLAGS) $(LIB_FLAGS) $(OBJS) $(LFT) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

lft:
	$(MAKE) bonus -C $(LFT_DIR)

$(LFT): lft

clean:
	$(MAKE) clean -C $(LFT_DIR)
	rm -f $(OBJS) objs
	
fclean: clean
	$(MAKE) fclean -C $(LFT_DIR)
	rm -f $(NAME)

re: fclean all
