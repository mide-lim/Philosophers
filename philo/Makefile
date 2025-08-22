NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

SRC = src/main.c src/utils/parsing.c src/utils/philo.c src/utils/time.c
OBJ = $(SRC:.c=.o)

INCLUDES = -I ../includes

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

src/%.o: src/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
