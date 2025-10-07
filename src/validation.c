/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 17:04:35 by musajid           #+#    #+#             */
/*   Updated: 2025/10/05 19:15:57 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

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
