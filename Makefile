NAME	= philo

CC		= gcc
CFLAGS	= -g -Wall -Wextra -Werror -pthread

SRCS	= src/main.c src/parse.c src/init.c src/routine.c \
		  src/monitor.c src/print.c src/utils.c src/dead.c \
		  src/cleanup.c

OBJS	= $(SRCS:.c=.o)

INCS	= -Iincludes

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(INCS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
