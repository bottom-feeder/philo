# === Variables ===
#CFLAGS  = -Wall -Wextra -Werror -g -O1 -fno-omit-frame-pointer -pthread -fsanitize=thread
CFLAGS  = -Wall -Wextra -Werror -pthread -g
#TSAN_OPTIONS="verbosity=1" setarch "$(uname -m)" -R ./philo [args]

NAME    = philo

SRCS    = error_cleanup.c \
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