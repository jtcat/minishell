SRCS	= ft_strlen.c ft_strlcat.c ft_strnstr.c ft_memcpy.c ft_isalpha.c ft_isdigit.c\
ft_isalnum.c ft_isascii.c ft_isprint.c ft_memset.c ft_bzero.c ft_memmove.c ft_strlcpy.c\
ft_toupper.c ft_tolower.c ft_strchr.c ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c\
ft_atoi.c ft_calloc.c ft_strdup.c ft_strtrim.c ft_substr.c ft_strjoin.c ft_split.c\
ft_itoa.c ft_strmapi.c ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c\
ft_putnbr_fd.c ft_strcmp.c ft_printf.c ft_procmulti.c ft_procnumb.c

BSRCS	= ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c ft_lstadd_back.c\
		  ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c

INC		= libft.h

MAIN	= main.c

NAME	= libft.a

COMP	= cc

OBJS	= ${SRCS:.c=.o}

BOBJS	= ${BSRCS:.c=.o}

FLAGS	= -Wall -Wextra -Werror

RM		= rm -f

%.o :		%.c
			${COMP} -c ${FLAGS} -o $@ $<

${NAME}:	${INC} ${OBJS}
			ar rc ${NAME} ${OBJS}

bonus:		${BOBJS} ${NAME}
			ar r ${NAME} ${BOBJS}
			touch bonus

all:		${NAME}

clean:	
			${RM} ${OBJS} ${BOBJS}

fclean:		clean
			${RM} ${NAME} ${T_NAME}

re:			fclean all
