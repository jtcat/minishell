CC:= cc

CFLAGS:= -O0 -g -Wall -Wextra -Werror

LIB_FLAGS:= -lreadline

NAME:= minishell

LFT_DIR:= src/libft
LFT:= $(LFT_DIR)/libft.a

SRCS:= $(addprefix src/, \
	main.c \
	$(addprefix lexer/, lexer.c) \
	$(addprefix utils/, str_utils.c) \
	$(addprefix parser/, grammar.c grammar2.c parser_utils.c) \
	$(addprefix exec/, exec.c exec2.c expan.c redir.c) \
	$(addprefix env/, env.c env2.c) \
	$(addprefix builtins/, builtins.c builtins2.c) \
	$(addprefix dl_list/, ft_dlstadd_back.c ft_dlstclear.c ft_dlstdelone.c ft_dlstlast.c ft_dlstnew.c ft_dlstrmone.c ft_dlstdup.c) \
	$(addprefix sig/, sig.c) \
)

INC:= $(addprefix -I, src/ src/exec src/env src/libft src/parser src/lexer src/utils src/dl_list src/sig)
 
HDR:= $(addprefix src/, minishell.h exec/exec.h env/env.h parser/parser.h lexer/lexer.h utils/shell_utils.h dl_list/dl_list.h sig/sig.h)

OBJS:= $(SRCS:.c=.o)

all: $(NAME)

bonus: $(NAME)

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
