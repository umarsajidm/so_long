#include "so_long.h"
#include <stdio.h>

static char	*read_file(char *filename)
{
	int		fd, bytes;
	char	buf[1024 + 1];
	char	*content, *tmp;

	content = ft_strdup("");
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		exit(write(2, "Error\nCannot open map\n", 22));
	while ((bytes = read(fd, buf, 1024)) > 0)
	{
		buf[bytes] = '\0';
		tmp = content;
		content = ft_strjoin(tmp, buf);
		free(tmp);
	}
	close(fd);
	return (content);
}

static int	count_rows(char *str)
{
	int count = 0;
	while (*str)
		if (*str++ == '\n')
			count++;
	return (count + 1);
}

char	**load_map(char *filename, t_game *game)
{
	char	*file;
	char	**lines;
	int		i;

	file = read_file(filename);
	game->height = count_rows(file);
	lines = ft_split(file, '\n');
	free(file);
	if (!lines || !lines[0])
		exit(write(2, "Error\nEmpty map\n", 16));
	game->width = ft_strlen(lines[0]);
	i = 0;
	while (lines[i])
	{
		if ((int)ft_strlen(lines[i]) != game->width)
			exit(write(2, "Error\nMap not rectangular\n", 26));
		i++;
	}
	game->map = lines;
	return (lines);
}

void	free_map(char **map)
{
	int i = 0;
	while (map && map[i])
		free(map[i++]);
	free(map);
}
