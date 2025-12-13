/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_utils1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkabang <stkabang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 19:05:35 by stkabang          #+#    #+#             */
/*   Updated: 2025/12/13 19:15:26 by stkabang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubd3.h"

static double	calculate_delta_dist(double ray_dir)
{
	if (ray_dir == 0)
		return 1e30;
	return (ft_abs_double(1.0 / ray_dir));
}

static void	init_step_side_x(t_ray *ray, t_player *player)
{
	if (ray->ray_dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (player->pos_x - ray->map_x) * ray->delta_dist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1.0 - player->pos_x) * ray->delta_dist_x;
	}
}

static void	init_step_side_y(t_ray *ray, t_player *player)
{
	if (ray->ray_dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (player->pos_y - ray->map_y) * ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0 - player->pos_y) * ray->delta_dist_y;
	}
}

void	init_ray(t_ray *ray, t_player *player, int x)
{
	double	camera_x;

	camera_x = 2.0 * x / WIDTH - 1.0;
	ray->ray_dir_x = player->dir_x + player->plane_x * camera_x;
	ray->ray_dir_y = player->dir_y + player->plane_y * camera_x;
	ray->map_x = (int)player->pos_x;
	ray->map_y = (int)player->pos_y;
	ray->delta_dist_x = calculate_delta_dist(ray->ray_dir_x);
	ray->delta_dist_y = calculate_delta_dist(ray->ray_dir_y);
	init_step_side_x(ray, player);
	init_step_side_y(ray, player);
	ray->hit = 0;
}


