#ifndef SO_LONG_H
# define SO_LONG_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include "libft/libft.h"
# include "minilibx-linux/mlx.h"

typedef struct s_game
{
	void	*mlx;
	void	*win;
	char	**map;
	int		width;
	int		height;
	int		player_x;
	int		player_y;
	int		moves;
	int		collectibles;
}	t_game;

// map.c
char	**load_map(char *filename, t_game *game);
void	free_map(char **map);

// validate.c
void	validate_map(t_game *game);

// render.c
void	render_map(t_game *game);

// controls.c
int		handle_key(int keycode, t_game *game);
int		close_game(t_game *game);


#endif
