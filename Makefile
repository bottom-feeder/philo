# === Variables ===
CFLAGS  = -Wall -Wextra -Werror -pthread -g

NAME    = philo

SRCS    = error_cleanup.c \
          forks.c \
          init.c \
          main.c \
          routines.c \
          simulation.c \
          utils.c

OBJS    = $(SRCS:.c=.o)

# === Rules ===
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re