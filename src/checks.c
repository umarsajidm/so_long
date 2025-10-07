/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 16:29:59 by musajid           #+#    #+#             */
/*   Updated: 2025/10/07 14:45:28 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	check_map_conditions(t_map *map)
{
	if (map->exit != 1 || map->collectibles == 0)
		return (-3);
	if (!validate_walls(map))
		return (-1);
	if (!validate_path(map))
		return (-2);
	return (1);
}

bool	image_failed(t_map *map)
{
	if (!map->wall_img || !map->counter_img || !map->floor_img
		|| !map->player_img || !map->exit_img || !map->collectible_img
		|| !map->enemy_img || !map->player_on_exit_img)
		return (true);
	return (false);
}
