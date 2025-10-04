# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <stdbool.h>
#include "mlx.h"
# include "libft.h"

typedef struct s_point
{
	int	x;
	int	y;
}				t_point;

typedef struct s_enemy
{
	int	x;
	int	y;
	int	direction;
}				t_enemy;

typedef struct s_map
{
	char	**grid;
	int		rows;
	int		cols;
	int		player;
	int		exit;
	int		collectibles;
	int		enemy;
	int		player_x;
	int		player_y;
	int		exit_x;
	int		exit_y;
	int		collectible_x;
	int		collectible_y;
	void	*mlx;
	void	*win;
	void	*wall_img;
	void	*floor_img;
	void	*player_img;
	void	*exit_img;
	void	*collectible_img;
	void	*player_on_exit_img;
	void	*enemy_img;
	void	*counter_img;
	int		img_w;
	int		img_h;
	int		move_count;
	t_enemy	*enemies;
	int		*enemy_x;
	int		*enemy_y;
	int		enemy_count;
}				t_map;

// so_long.c
int		main(int ac, char **av);

// map_parsing.c
int		count_lines(int fd);
int		parse_map(t_map *map, int fd);

// map_validate.c
int		validate_path(t_map *map);
int		validate_walls(t_map *map);

// map_utils.c
void	free_map(t_map *map);
void	free_grid(char **map, int height);
int		parse_error(t_map *map, char *msg);
void	flood_fill(char **map, int width, int height, t_point pos);
char	**duplicate_map(char **map, int height);

// game.c
void	start_game(t_map *map);
int		close_game(t_map *map);
int		game_state(t_map *map, int x, int y, int *moves);

// player_control.c
void	move_player(t_map *map, int dx, int dy);
int		handle_key(int keycode, t_map *map);

// render.c
void	init_images(t_map *map);
void	render_map(t_map *map);

/////game.c

/* 
** start_game:
** Initializes the MiniLibX connection, creates the window with map dimensions,
** loads all sprites, and renders the initial state of the map.
** It also sets up input hooks and starts the main mlx loop.
*/
void	start_game(t_map *map)
{
	map->mlx = mlx_init();
	map->win = mlx_new_window(
			map->mlx,
			map->cols * 64,
			map->rows * 64,
			"so_long");
	init_images(map);
	render_map(map);
	mlx_key_hook(map->win, handle_key, map);
	mlx_hook(map->win, 17, 0, close_game, map);
	mlx_loop(map->mlx);
}

/*
** close_game:
** Frees all allocated resources (map, mlx images, window, and display).
** This is called when the user exits the game or the game ends.
*/

int	close_game(t_map *map)
{
	if (map->wall_img)
		mlx_destroy_image(map->mlx, map->wall_img);
	if (map->counter_img)
		mlx_destroy_image(map->mlx, map->counter_img);
	if (map->floor_img)
		mlx_destroy_image(map->mlx, map->floor_img);
	if (map->player_img)
		mlx_destroy_image(map->mlx, map->player_img);
	if (map->exit_img)
		mlx_destroy_image(map->mlx, map->exit_img);
	if (map->enemy_img)
		mlx_destroy_image(map->mlx, map->enemy_img);
	if (map->collectible_img)
		mlx_destroy_image(map->mlx, map->collectible_img);
	if (map->player_on_exit_img)
		mlx_destroy_image(map->mlx, map->player_on_exit_img);
	free_map(map);
	if (map->win || map->mlx)
	{
		if (map->win)
			mlx_destroy_window(map->mlx, map->win);
		mlx_destroy_display(map->mlx);
		free(map->mlx);
	}
	exit(0);
}

/*
** game_state:
** Checks the current state after a player move.
** Detects if the player reached an exit with all collectibles (win) 
** or if collided with an enemy (lose).
*/

int	game_state(t_map *map, int x, int y, int *moves)
{
	char	tile;
	int		i;

	tile = map->grid[y][x];
	i = 0;
	while (i < map->enemy_count)
	{
		if (map->enemy_x[i] == x && map->enemy_y[i] == y)
		{
			(*moves)++;
			ft_printf("Moves: %d\nYou lose!\n", *moves);
			close_game(map);
			return (1);
		}
		i++;
	}
	if (tile == 'E' && map->collectibles == 0)
	{
		(*moves)++;
		ft_printf("You WIN!\nTotal moves: %d\n", *moves);
		close_game(map);
		return (1);
	}
	return (0);
}

////map_parsing.c


/*
** count_lines:
** Counts the number of lines in the map file.
** Returns -1 if empty lines are detected (invalid map).
*/

int	count_lines(int fd)
{
	int		count;
	char	*line;
	char	*prev;

	count = 0;
	prev = NULL;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (line[0] == '\n')
		{
			free(line);
			free(prev);
			return (-1);
		}
		free(prev);
		prev = line;
		count++;
	}
	if (prev && prev[ft_strlen(prev) - 1] == '\n')
		return (free(prev), -1);
	free(prev);
	return (count);
}



static int	validate_chars(char *line, int row, t_map *map)
{
	int	i;

	i = 0;
	while (line[i] && line[i] != '\n')
	{
		if (line[i] == 'P')
			map->player++;
		else if (line[i] == 'E')
			map->exit++;
		else if (line[i] == 'C')
			map->collectibles++;
		else if (line[i] == 'N')
			map->enemy++;
		else if (line[i] != '0' && line[i] != '1')
		{
			ft_printf("Invalid character '%c' at row %d, col %d\n",
				line[i], row, i);
			return (0);
		}
		i++;
	}
	return (i);
}

static int	validate_line(char *line, t_map *map, int row)
{
	int	len;

	len = validate_chars(line, row, map);
	if (len == 0)
		return (0);
	if (row == 0)
		map->cols = len;
	else if (len != map->cols)
		return (0);
	if (map->player > 1 || map->exit > 1)
		return (0);
	return (1);
}

/*
** parse_map:
** Reads each line of the map into the map->grid array.
** Validates characters, line lengths, and ensures only one player and one exit.
** Also calls wall and path validation at the end.
*/

int	parse_map(t_map *map, int fd)
{
	char	*line;
	int		i;

	i = 0;
	map->player = 0;
	map->exit = 0;
	map->collectibles = 0;
	line = get_next_line(fd);
	while (line)
	{
		if (!validate_line(line, map, i))
		{
			free(line);
			return (0);
		}
		map->grid[i] = line;
		i++;
		line = get_next_line(fd);
	}
	if (!validate_walls(map))
		return (-1);
	if (!validate_path(map))
		return (-2);
	return (1);
}

//map_utils.c


void	free_map(t_map *map)
{
	int	i;

	if (map->grid)
	{
		i = 0;
		while (i < map->rows)
		{
			if (map->grid[i])
			{
				free(map->grid[i]);
				map->grid[i] = NULL;
			}
			i++;
		}
		free(map->grid);
		map->grid = NULL;
	}
}

void	free_grid(char **map, int height)
{
	int	i;

	i = 0;
	while (i < height)
	{
		free(map[i]);
		i++;
	}
	free(map);
}

int	parse_error(t_map *map, char *msg)
{
	if (map)
		free_map(map);
	if (msg)
		ft_printf("Error: %s\n", msg);
	else
		ft_printf("Error\n");
	return (0);
}

void	flood_fill(char **map, int width, int height, t_point pos)
{
	const int	dx[4] = {1, -1, 0, 0};
	const int	dy[4] = {0, 0, 1, -1};
	int			i;
	t_point		next;

	if (pos.x < 0 || pos.y < 0 || pos.x >= width || pos.y >= height)
		return ;
	if (map[pos.y][pos.x] == '1' || map[pos.y][pos.x] == 'V'
		|| map[pos.y][pos.x] == 'N')
		return ;
	map[pos.y][pos.x] = 'V';
	i = 0;
	while (i < 4)
	{
		next.x = pos.x + dx[i];
		next.y = pos.y + dy[i];
		flood_fill(map, width, height, next);
		i++;
	}
}

char	**duplicate_map(char **map, int height)
{
	char	**copy;
	int		i;

	copy = malloc(sizeof(char *) * (height + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < height)
	{
		copy[i] = ft_strdup(map[i]);
		if (!copy[i])
		{
			while (--i >= 0)
				free(copy[i]);
			free(copy);
			return (NULL);
		}
		i++;
	}
	copy[height] = NULL;
	return (copy);
}

/////map_validate.c

static void	update_position(t_map *map, char c, int i, int j)
{
	if (c == 'P')
	{
		map->player_y = i;
		map->player_x = j;
	}
	else if (c == 'E')
	{
		map->exit_y = i;
		map->exit_x = j;
	}
	else if (c == 'C')
	{
		map->collectible_y = i;
		map->collectible_x = j;
	}
}

static void	set_positions(t_map *map)
{
	int	i;
	int	j;

	i = 0;
	while (i < map->rows)
	{
		j = 0;
		while (j < map->cols)
		{
			update_position(map, map->grid[i][j], i, j);
			j++;
		}
		i++;
	}
}

static bool	check_reachability(char **map, int width, int height)
{
	int	x;
	int	y;

	y = 0;
	while (y < height)
	{
		x = 0;
		while (x < width)
		{
			if (map[y][x] == 'C' || map[y][x] == 'E')
				return (false);
			x++;
		}
		y++;
	}
	return (true);
}

int	validate_path(t_map *map)
{
	t_point	start;
	char	**map_copy;
	bool	valid;

	set_positions(map);
	start.x = map->player_x;
	start.y = map->player_y;
	map_copy = duplicate_map(map->grid, map->rows);
	if (!map_copy)
		return (0);
	flood_fill(map_copy, map->cols, map->rows, start);
	valid = check_reachability(map_copy, map->cols, map->rows);
	free_grid(map_copy, map->rows);
	return (valid);
}

int	validate_walls(t_map *map)
{
	int	i;

	i = 0;
	while (i < map->cols)
	{
		if (map->grid[0][i] != '1' || map->grid[map->rows - 1][i] != '1')
			return (0);
		i++;
	}
	i = 0;
	while (i < map->rows)
	{
		if (map->grid[i][0] != '1' || map->grid[i][map->cols - 1] != '1')
			return (0);
		i++;
	}
	return (1);
}
////player_control.c

static void	update_player_position(t_map *map, int x, int y, char tile)
{
	if (tile == 'C')
		map->collectibles--;
	if (map->grid[map->player_y][map->player_x] != 'E')
		map->grid[map->player_y][map->player_x] = '0';
	if (tile != 'E')
		map->grid[y][x] = 'P';
	map->player_x = x;
	map->player_y = y;
}

void	move_player(t_map *map, int dx, int dy)
{
	int		x;
	int		y;
	char	tile;

	x = map->player_x + dx;
	y = map->player_y + dy;
	tile = map->grid[y][x];
	if (tile == '1')
		return ;
	if (tile == 'N')
	{
		map->move_count++;
		ft_printf("Moves: %d\nYou lose!\n", map->move_count);
		close_game(map);
		return ;
	}
	if (game_state(map, x, y, &map->move_count))
		return ;
	update_player_position(map, x, y, tile);
	map->move_count++;
	render_map(map);
}

int	handle_key(int keycode, t_map *map)
{
	if (keycode == 65307)
		close_game(map);
	else if (keycode == 'W' || keycode == 'w' || keycode == 65362)
		move_player(map, 0, -1);
	else if (keycode == 'S' || keycode == 's' || keycode == 65364)
		move_player(map, 0, 1);
	else if (keycode == 'A' || keycode == 'a' || keycode == 65361)
		move_player(map, -1, 0);
	else if (keycode == 'D' || keycode == 'd' || keycode == 65363)
		move_player(map, 1, 0);
	return (0);
}

///render.c

void	init_images(t_map *map)
{
	map->wall_img = NULL;
	map->counter_img = NULL;
	map->floor_img = NULL;
	map->player_img = NULL;
	map->exit_img = NULL;
	map->collectible_img = NULL;
	map->player_on_exit_img = NULL;
	map->enemy_img = NULL;
	map->wall_img = mlx_xpm_file_to_image(map->mlx,
			"resources/images/wall.xpm", &map->img_w, &map->img_h);
	map->counter_img = mlx_xpm_file_to_image(map->mlx,
			"resources/images/counter.xpm", &map->img_w, &map->img_h);
	map->floor_img = mlx_xpm_file_to_image(map->mlx,
			"resources/images/floor.xpm", &map->img_w, &map->img_h);
	map->player_img = mlx_xpm_file_to_image(map->mlx,
			"resources/images/player.xpm", &map->img_w, &map->img_h);
	map->exit_img = mlx_xpm_file_to_image(map->mlx,
			"resources/images/exit.xpm", &map->img_w, &map->img_h);
	map->collectible_img = mlx_xpm_file_to_image(map->mlx,
			"resources/images/collectible.xpm", &map->img_w, &map->img_h);
	map->player_on_exit_img = mlx_xpm_file_to_image(map->mlx,
			"resources/images/p&e.xpm", &map->img_w, &map->img_h);
	map->enemy_img = mlx_xpm_file_to_image(map->mlx,
			"resources/images/enemy.xpm", &map->img_w, &map->img_h);
}

static void	render_tile(t_map *map, int x, int y, void *img)
{
	mlx_put_image_to_window(map->mlx, map->win, img, x * 64, y * 64);
}

static void	render_tile_with_player(t_map *map, int x, int y, char tile)
{
	if (tile == '1')
	{
		if (x == 0 && y == 0)
			render_tile(map, x, y, map->counter_img);
		else
			render_tile(map, x, y, map->wall_img);
	}
	else
		render_tile(map, x, y, map->floor_img);
	if (tile == 'C')
		render_tile(map, x, y, map->collectible_img);
	if (tile == 'E')
		render_tile(map, x, y, map->exit_img);
	if (tile == 'N')
		render_tile(map, x, y, map->enemy_img);
	if (x == map->player_x && y == map->player_y)
	{
		if (tile == 'E' && map->collectibles > 0)
			render_tile(map, x, y, map->player_on_exit_img);
		else
			render_tile(map, x, y, map->player_img);
	}
}

static void	render_counter(t_map *map, int moves)
{
	char	*moves_str;
	int		y_offset;

	mlx_string_put(map->mlx, map->win, 10, 20, 0xFF0000, "Moves:");
	moves_str = ft_itoa(moves);
	if (moves_str)
	{
		y_offset = 40;
		mlx_string_put(map->mlx, map->win, 10, y_offset, 0xFF0000, moves_str);
		free(moves_str);
	}
}

void	render_map(t_map *map)
{
	int	x;
	int	y;

	y = 0;
	while (y < map->rows)
	{
		x = 0;
		while (x < map->cols)
		{
			render_tile_with_player(map, x, y, map->grid[y][x]);
			x++;
		}
		y++;
	}
	render_counter(map, map->move_count);
}

///so_long.c

static int	valid_ext(const char *filename)
{
	size_t	len;

	if (!filename)
		return (0);
	len = ft_strlen(filename);
	if (len < 4)
		return (0);
	return (ft_strncmp(filename + len - 4, ".ber", 4) == 0);
}

int	init_map(t_map *map, char *filename)
{
	int	fd;
	int	i;

	ft_memset(map, 0, sizeof(t_map));
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (parse_error(map, "Cannot open file"));
	map->rows = count_lines(fd);
	close(fd);
	if (map->rows == -1)
		return (parse_error(map, "Empty line on map"));
	if (map->rows == 0)
		return (parse_error(map, "Empty map file"));
	map->grid = malloc(sizeof(char *) * map->rows);
	if (!map->grid)
		return (parse_error(map, "Malloc failed"));
	i = 0;
	while (i < map->rows)
	{
		map->grid[i] = NULL;
		i++;
	}
	return (1);
}

static int	process_map(t_map *map, char *filename)
{
	int	fd;
	int	res;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (parse_error(map, "File open failed"));
	res = parse_map(map, fd);
	close(fd);
	if (res == 1)
		return (1);
	else if (res == 0)
		return (parse_error(map, "Invalid character or line"));
	else if (res == -1)
		return (parse_error(map, "Invalid wall"));
	else if (res == -2)
		return (parse_error(map, "Invalid path"));
	return (parse_error(map, ""));
}

int	main(int ac, char **av)
{
	t_map	map;

	ft_memset(&map, 0, sizeof(t_map));
	if (ac != 2)
		return (parse_error(&map, "Invalid args"));
	if (!valid_ext(av[1]))
		return (parse_error(&map, "Invalid extension"));
	if (!init_map(&map, av[1]))
		return (0);
	if (!process_map(&map, av[1]))
		return (0);
	start_game(&map);
	free_map(&map);
	return (0);
}