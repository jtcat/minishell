CC:= cc

CFLAGS:= -O0 -g -Wall -Wextra

LIB_FLAGS:= -lreadline

INC_DIR:= include

NAME:= minishell

LFT_DIR:= libft
LFT:= $(LFT_DIR)/libft.a

SRCS = src/main.c \
	$(addprefix lexer/, lexer.c) \
	$(addprefix utils/, str_utils.c) \
	$(addprefix parser/, grammar.c grammar2.c parser_utils.c) \
	$(addprefix exec/, exec.c) \
	$(addprefix builtins/, builtins.c) \

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(LFT) $(OBJS)
	$(CC) $(CFLAGS) $(LIB_FLAGS) $(OBJS) $(LFT) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

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
