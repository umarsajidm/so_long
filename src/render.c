/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 17:05:55 by musajid           #+#    #+#             */
/*   Updated: 2025/10/05 17:05:55 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	init_images(t_map *map)
{
	map->wall_img = mlx_xpm_file_to_image(map->mlx,
			"textures/wall.xpm", &map->img_w, &map->img_h);
	map->counter_img = mlx_xpm_file_to_image(map->mlx,
			"textures/counter.xpm", &map->img_w, &map->img_h);
	map->floor_img = mlx_xpm_file_to_image(map->mlx,
			"textures/floor.xpm", &map->img_w, &map->img_h);
	map->player_img = mlx_xpm_file_to_image(map->mlx,
			"textures/player.xpm", &map->img_w, &map->img_h);
	map->exit_img = mlx_xpm_file_to_image(map->mlx,
			"textures/exit.xpm", &map->img_w, &map->img_h);
	map->collectible_img = mlx_xpm_file_to_image(map->mlx,
			"textures/collectible.xpm", &map->img_w, &map->img_h);
	map->player_on_exit_img = mlx_xpm_file_to_image(map->mlx,
			"textures/p&e.xpm", &map->img_w, &map->img_h);
	map->enemy_img = mlx_xpm_file_to_image(map->mlx,
			"textures/enemy.xpm", &map->img_w, &map->img_h);
	if (image_failed(map))
	{
		parse_error(map, "Failed to load image");
		close_game(map);
	}
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
