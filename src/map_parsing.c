/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 17:02:33 by musajid           #+#    #+#             */
/*   Updated: 2025/10/07 15:01:13 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

/*
** count_lines:
** Counts the number of lines in the map file.
** Returns -1 if empty lines are detected (invalid map).
*/

static int	free_and_return(char *line, char *last, int ret)
{
	free(line);
	free(last);
	return (ret);
}

int	count_lines(int fd)
{
	int		count;
	char	*line;
	char	*last;

	count = 0;
	last = NULL;
	line = get_next_line(fd);
	while (line)
	{
		if (line[0] == '\n')
			return (free_and_return(line, last, -1));
		free(last);
		last = line;
		count++;
		line = get_next_line(fd);
	}
	if (count == 0)
	{
		free(last);
		return (0);
	}
	if (!last && last[ft_strlen(last) - 1] == '\n')
		return (free_and_return(NULL, last, -1));
	free(last);
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
		map->grid[i++] = line;
		line = get_next_line(fd);
	}
	return (check_map_conditions(map));
}
