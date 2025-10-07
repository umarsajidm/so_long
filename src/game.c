/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 17:01:27 by musajid           #+#    #+#             */
/*   Updated: 2025/10/05 17:01:56 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

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
