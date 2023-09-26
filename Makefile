CC:= cc

CFLAGS:= -g -Wall -Wextra -Werror

LIB_FLAGS:= -lreadline

INC_DIR:= include

NAME:= minishell

LFT_DIR:= src/libft
LFT:= $(LFT_DIR)/libft.a

SRCS:= $(addprefix src/, \
	main.c \
	$(addprefix lexer/, lexer.c) \
	$(addprefix utils/, str_utils.c) \
	$(addprefix parser/, grammar.c grammar2.c parser_utils.c) \
	$(addprefix exec/, exec.c exec2.c expan.c redir.c) \
	$(addprefix builtins/, builtins.c) \
)

HDR:= src/exec/exec.h
 
INC:= $(addprefix $(INC_DIR)/, minishell.h utils.h)

OBJS:= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(HDR) $(INC) $(LFT) $(OBJS)
	$(CC) $(CFLAGS) $(LIB_FLAGS) $(OBJS) $(LFT) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

lft:
	$(MAKE) bonus -C $(LFT_DIR)
	cp $(LFT_DIR)/libft.h $(INC_DIR)

$(LFT): lft

clean:
	$(MAKE) clean -C $(LFT_DIR)
	rm -f $(OBJS) objs
	
fclean: clean
	$(MAKE) fclean -C $(LFT_DIR)
	rm -f $(NAME)

re: fclean all
