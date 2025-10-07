/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 17:05:18 by musajid           #+#    #+#             */
/*   Updated: 2025/10/06 16:12:50 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

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
	ft_printf("Moves: %d\n", map->move_count);
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
