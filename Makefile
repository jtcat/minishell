CC:= cc

CFLAGS:= -Wall -Werror -Wextra

LIB_FLAGS:= -pthread -lradline

NAME:= minishell

LFT_DIR:= libft
LFT:= $(LFT_DIR)/libft.a

SRCS = main.c ./Lexer/Lexer.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) $(LFT)	
	$(CC) $(CFLAGS) $(LIB_FLAGS) $(LFT) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LFT):
	$(MAKE) bonus -C $(LFT_DIR)

clean:
	$(MAKE) clean -C $(LFT_DIR)
	rm -f $(OBJS) objs
	
fclean: clean
	$(MAKE) fclean -C $(LFT_DIR)
	rm -f $(NAME)

re: fclean all
