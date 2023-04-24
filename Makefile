CC = cc -g
CFLAGS = -Wall -Werror -Wextra -pthread -lradline
NAME = minishell

SRCS = main.c ./Lexer/Lexer.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

${NAME}: ${OBJS}	
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS) objs
	
fclean: clean
	rm -f $(NAME)

re: fclean all