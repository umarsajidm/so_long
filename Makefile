NAME = so_long

CC = cc

CFLAGS = -Wall -Wextra -Werror -I. -I$(MLXDIR)

SRC = src/main.c \
	src/game.c \
	src/validation.c \
	src/render.c \
	src/control.c \
	src/utils.c	\
	src/map_parsing.c \
	src/checks.c

OBJ = $(SRC:.c=.o)

LIBFTDIR = libft
LIBFT = $(LIBFTDIR)/libft.a

MLXDIR = minilibx-linux
MLX = -L$(MLXDIR) -lmlx -lXext -lX11 -lm -lbsd

all: $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFTDIR)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(MLX) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(MAKE) clean -C $(LIBFTDIR)
	rm -f $(OBJ)

fclean:
	$(MAKE) fclean -C $(LIBFTDIR)
	rm -f $(NAME) $(OBJ)

re: fclean all

.PHONY: all clean fclean re
