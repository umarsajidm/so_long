#include "so_long.h"

static void	put_tile(t_game *g, int x, int y, char tile)
{
	if (tile == '1')
		mlx_string_put(g->mlx, g->win, x * 32, y * 32, 0xFFFFFF, "#");
	else if (tile == '0')
		mlx_string_put(g->mlx, g->win, x * 32, y * 32, 0xFFFFFF, ".");
	else if (tile == 'P')
		mlx_string_put(g->mlx, g->win, x * 32, y * 32, 0x00FF00, "P");
	else if (tile == 'E')
		mlx_string_put(g->mlx, g->win, x * 32, y * 32, 0xFF0000, "E");
	else if (tile == 'C')
		mlx_string_put(g->mlx, g->win, x * 32, y * 32, 0xFFFF00, "C");
}

void	render_map(t_game *g)
{
	int	y;
	int	x;

	y = 0;
	while (y < g->height)
	{
		x = 0;
		while (x < g->width)
		{
			put_tile(g, x, y, g->map[y][x]);
			x++;
		}
		y++;
	}
}
