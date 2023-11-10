CC:= cc

CFLAGS:= -O0 -g -Wall -Wextra -Werror
#CFLAGS:= -fsanitize=address -O0 -g -Wall -Wextra -Werror

LIB_FLAGS:= -lreadline

NAME:= minishell

LFT_DIR:= src/libft
LFT:= $(LFT_DIR)/libft.a

SRC_COMMON:= $(addprefix src/, \
	main.c \
	$(addprefix parser/, grammar.c grammar2.c parser_utils.c hdoc.c) \
	$(addprefix exec/, exec.c exec2.c exec3.c expan.c redir.c) \
	$(addprefix env/, env.c env2.c) \
	$(addprefix builtins/, builtins.c builtins2.c builtins3.c) \
	$(addprefix dl_list/, ft_dlstadd_back.c ft_dlstclear.c ft_dlstdelone.c ft_dlstlast.c ft_dlstnew.c ft_dlstrmone.c ft_dlstdup.c) \
	$(addprefix sig/, sig.c) \
	$(addprefix gnl/, gnl.c) \
)

SRC_BASE:= $(SRC_COMMON) $(addprefix src/, lexer/lexer.c utils/str_utils.c)

SRC_BONUS:= $(SRC_COMMON) $(addprefix src_bonus/, lexer/lexer_bonus.c utils/str_utils_bonus.c)

INC:= $(addprefix -I, src/ src/exec src/env src/libft src/parser src/lexer src/utils src/dl_list src/sig src/gnl)
 
HDR:= $(addprefix src/, minishell.h exec/exec.h env/env.h parser/parser.h lexer/lexer.h utils/shell_utils.h dl_list/dl_list.h sig/sig.h gnl/gnl.h builtins/builtins.h)

OBJS:= $(SRC_BASE:.c=.o) 

OBJS_BONUS:= $(SRC_BONUS:.c=.o)

all: $(NAME)

bonus: $(HDR) $(LFT) $(OBJS_BONUS)
	$(CC) $(CFLAGS) $(LIB_FLAGS) $(OBJS_BONUS) $(LFT) -o $(NAME)

$(NAME): $(HDR) $(LFT) $(OBJS)
	$(CC) $(CFLAGS) $(LIB_FLAGS) $(OBJS) $(LFT) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

lft:
	$(MAKE) bonus -C $(LFT_DIR)

$(LFT): lft

clean:
	$(MAKE) clean -C $(LFT_DIR)
	rm -f $(OBJS)
	rm -f $(OBJS_BONUS)
	
fclean: clean
	$(MAKE) fclean -C $(LFT_DIR)
	rm -f $(NAME)

re: fclean all
