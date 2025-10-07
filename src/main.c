/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 17:06:47 by musajid           #+#    #+#             */
/*   Updated: 2025/10/05 17:06:47 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

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
	else if (res == -3)
		return (parse_error(map, "At least one collectable or exit"));
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
