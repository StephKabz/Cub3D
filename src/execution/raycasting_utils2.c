/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkabang <stkabang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 19:05:49 by stkabang          #+#    #+#             */
/*   Updated: 2025/12/13 19:16:41 by stkabang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubd3.h"

void	do_DDA(t_ray *ray, t_map *map)
{
	while (ray->hit == 0)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (map->lines_map[ray->map_y][ray->map_x] == '1')
			ray->hit = 1;
	}
}

void	calculate_perp_dist(t_ray * ray, t_player *player)
{
	if (ray->side == 0)
		ray->perp_dist_wall = (ray->map_x - player->pos_x + (1 - ray->step_x) / 2) / ray->ray_dir_x;
	else
		ray->perp_dist_wall = (ray->map_y - player->pos_y + (1 - ray->step_y) / 2) / ray->ray_dir_y;
}

void	floor_ceiling_color(t_game *game, int *floor, int *ceiling)
{
	*floor = (game->scene.fc.floor.red << 16)
				| (game->scene.fc.floor.green << 8)
				| (game->scene.fc.floor.blue);
	*ceiling = (game->scene.fc.ceiling.red << 16)
				| (game->scene.fc.ceiling.green << 8)
				| (game->scene.fc.ceiling.blue);
}

void	wall_bounds(t_ray *ray, int *draw_start, int *draw_end, double *line_height)
{
	*line_height = (double)(HEIGHT / ray->perp_dist_wall);
	*draw_start = (int)(-(*line_height) / 2 + HEIGHT / 2);
	if (*draw_start < 0)
		*draw_start = 0;
	*draw_end = (int)((*line_height) / 2 + HEIGHT / 2);
	if (*draw_end >= HEIGHT)
		*draw_end = HEIGHT - 1;
}

double	calculate_wall_x(t_game *game, t_ray *ray)
{
	double	wall_x;
	if (ray->side == 0)
	{
		wall_x = game->player.pos_y + ray->perp_dist_wall * ray->ray_dir_y;
	}
	else
		wall_x = game->player.pos_x + ray->perp_dist_wall * ray->ray_dir_x;
	wall_x -= ft_floor(wall_x);
	return (wall_x);
}