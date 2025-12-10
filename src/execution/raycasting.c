/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkabang <stkabang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 16:25:10 by stkabang          #+#    #+#             */
/*   Updated: 2025/12/09 19:44:00 by stkabang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubd3.h"

static double	ft_floor(double nb)
{
	if (nb >= 0)
		return ((double)(int)nb);
	if (nb != ((int)nb))
		return ((double)(int)nb - 1);
	return ((double)(int)nb);
}

static double	ft_abs_double(double nb)
{
	if (nb < 0)
		return (-nb);
	return (nb);
}

static void	init_ray(t_ray *ray, t_player *player, int x)
{
	double	camera_x;

	camera_x = 2.0 * x / WIDTH - 1.0;
	ray->ray_dir_x = player->dirX + player->planeX * camera_x;
	ray->ray_dir_y = player->dirY + player->planeY * camera_x;
	ray->map_x = (int)player->posX;
	ray->map_y = (int)player->posY;
	if (ray->ray_dir_x == 0)
		ray->delta_dist_x = 1e30;
	else
		ray->delta_dist_x = ft_abs_double(1.0 / ray->ray_dir_x);
	if (ray->ray_dir_y == 0)
		ray->delta_dist_y = 1e30;
	else
		ray->delta_dist_y = ft_abs_double(1.0 / ray->ray_dir_y);
	if (ray->ray_dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (player->posX - ray->map_x) * ray->delta_dist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1.0 - player->posX) * ray->delta_dist_x;
	}
	if (ray->ray_dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (player->posY - ray->map_y) * ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0 - player->posY) * ray->delta_dist_y;
	}
	ray->hit = 0;
}

static void	do_DDA(t_ray *ray, t_map *map)
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

static void	calculate_perp_dist(t_ray * ray, t_player *player)
{
	if (ray->side == 0)
		ray->perp_dist_wall = (ray->map_x - player->posX + (1 - ray->step_x) / 2) / ray->ray_dir_x;
	else
		ray->perp_dist_wall = (ray->map_y - player->posY + (1 - ray->step_y) / 2) / ray->ray_dir_y;
}

static void	draw_wall_column(t_game *game, t_ray *ray, int x)
{
	int				draw_start;
	int				draw_end;
	int				y;
	int				ceiling_color;
	int				floor_color;
	int				color;
	int				texX;
	int				texY;
	double			line_height;
	double			wallX;
	double			step;
	double			tex_pos;
	t_textures_img	*texture;
	

	floor_color = (game->scene.fc.floor.red << 16)
				| (game->scene.fc.floor.green << 8)
				| (game->scene.fc.floor.blue);
	ceiling_color = (game->scene.fc.ceiling.red << 16)
				| (game->scene.fc.ceiling.green << 8)
				| (game->scene.fc.ceiling.blue);
	line_height = (int)(HEIGHT / ray->perp_dist_wall);
	draw_start = -line_height / 2 + HEIGHT / 2;
	if (draw_start < 0)
		draw_start = 0;
	draw_end = line_height / 2 + HEIGHT / 2;
	if (draw_end >= HEIGHT)
		draw_end = HEIGHT - 1;
	if (ray->side == 0)
		wallX = game->player.posY + ray->perp_dist_wall * ray->ray_dir_y;
	else
		wallX = game->player.posX + ray->perp_dist_wall * ray->ray_dir_x;
	wallX -= ft_floor(wallX);
	if (ray->side == 0)
	{
		if (ray->ray_dir_x > 0)
			texture = &game->textures.east;
		else
			texture = &game->textures.west;
	}
	else
	{
		if (ray->ray_dir_y > 0)
			texture = &game->textures.south;
		else
			texture = &game->textures.north;
	}
	texX = (int)(wallX * (double)texture->width);
	if (texX < 0)
		texX = 0;
	if (texX >= texture->width)
		texX = texture->width - 1;
	step = 1.0 * texture->height / line_height;
	tex_pos = (draw_start - HEIGHT / 2 + line_height / 2) * step;
	y = 0;
	while (y < HEIGHT)
	{
		if (y < draw_start)
			put_pixel(game, x, y, ceiling_color);
		else if (y >= draw_start && y < draw_end)
		{
			texY = (int)tex_pos;
			if (texY < 0)
				texY = 0;
			if (texY >= texture->height)
				texY = texture->height - 1;
			color = *(int *)(texture->img_data + texY * texture->line_len + texX * (texture->bpp / 8));
			put_pixel(game, x, y, color);
			tex_pos += step;
		}
		else
			put_pixel(game, x, y, floor_color);
		y++;
	}
}

void	raycasting(t_game *game)
{
	int	x;

	x = 0;
	while (x < WIDTH)
	{
		t_ray ray;
		init_ray(&ray, &game->player, x);
		do_DDA(&ray, &game->scene.map);
		calculate_perp_dist(&ray, &game->player);
		draw_wall_column(game, &ray, x);
		x++;
	}
}
