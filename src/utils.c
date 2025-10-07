/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 17:04:03 by musajid           #+#    #+#             */
/*   Updated: 2025/10/07 14:41:32 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

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
	{
		ft_putstr_fd("Error\n", 2);
		ft_putstr_fd(msg, 2);
		ft_putstr_fd("\n", 2);
	}
	else
		ft_putstr_fd("Error\n", 2);
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
