/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 19:13:06 by musajid           #+#    #+#             */
/*   Updated: 2025/10/05 19:13:06 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <stdbool.h>
# include "minilibx-linux/mlx.h"
# include "libft/libft.h"

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

//checks.c
bool	image_failed(t_map *map);
int		check_map_conditions(t_map *map);

#endif
