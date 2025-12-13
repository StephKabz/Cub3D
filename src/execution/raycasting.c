/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkabang <stkabang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 16:25:10 by stkabang          #+#    #+#             */
/*   Updated: 2025/12/13 18:23:56 by stkabang         ###   ########.fr       */
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

static void	init_ray(t_ray *ray, t_player *player, int x)
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
		ray->perp_dist_wall = (ray->map_x - player->pos_x + (1 - ray->step_x) / 2) / ray->ray_dir_x;
	else
		ray->perp_dist_wall = (ray->map_y - player->pos_y + (1 - ray->step_y) / 2) / ray->ray_dir_y;
}

static void	floor_ceiling_color(t_game *game, int *floor, int *ceiling)
{
	*floor = (game->scene.fc.floor.red << 16)
				| (game->scene.fc.floor.green << 8)
				| (game->scene.fc.floor.blue);
	*ceiling = (game->scene.fc.ceiling.red << 16)
				| (game->scene.fc.ceiling.green << 8)
				| (game->scene.fc.ceiling.blue);
}

static void	wall_bounds(t_ray *ray, int *draw_start, int *draw_end, double *line_height)
{
	*line_height = (double)(HEIGHT / ray->perp_dist_wall);
	*draw_start = (int)(-(*line_height) / 2 + HEIGHT / 2);
	if (*draw_start < 0)
		*draw_start = 0;
	*draw_end = (int)((*line_height) / 2 + HEIGHT / 2);
	if (*draw_end >= HEIGHT)
		*draw_end = HEIGHT - 1;
}

static double	calculate_wall_x(t_game *game, t_ray *ray)
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

static t_textures_img *select_texture(t_game *game, t_ray *ray)
{
	if (ray->side == 0)
	{
		if (ray->ray_dir_x > 0)
			return (&game->textures.east);
		else
			return (&game->textures.west);
	}
	else
	{
		if (ray->ray_dir_y > 0)
			return (&game->textures.south);
		else
			return (&game->textures.north);
	}
}

static t_tex_params init_tex_rendering(t_textures_img *texture, double wall_x, double line_height, int draw_start)
{
	t_tex_params	params;

	params.tex_x = (int)(wall_x * (double)texture->width);
	if (params.tex_x < 0)
		params.tex_x = 0;
	if (params.tex_x >= texture->width)
		params.tex_x = texture->width - 1;
	params.step = 1.0 * texture->height / line_height;
	params.tex_pos = (draw_start - HEIGHT / 2 + line_height / 2) * params.step;
	return (params);
}


static void	draw_column(t_game *game, int x, t_render_params *params)
{
	int		y;
	int		tex_y;
	int		color;
	double	tex_pos;

	tex_pos = params->tex.tex_pos;
	y = 0;
	while(y < HEIGHT)
	{
		if (y < params->draw_start)
			put_pixel(game,x, y, params->ceiling_color);
		else if (y >= params->draw_start && y < params->draw_end)
		{
			tex_y = (int)tex_pos;
			if (tex_y < 0)
				tex_y = 0;
			if (tex_y >= params->texture->height)
				tex_y = params->texture->height - 1;
			color = *(int *)(params->texture->img_data
					+ tex_y * params->texture->line_len
					+ params->tex.tex_x * (params->texture->bpp / 8));
			put_pixel(game, x, y, color);
			tex_pos += params->tex.step;
		}
		else
			put_pixel(game, x, y, params->floor_color);
		y++;
	}
}

static void draw_wall_column(t_game *game, t_ray *ray, int x)
{
    t_render_params	params;
    double			wallX;
    double			line_height;
    
	floor_ceiling_color(game, &params.floor_color, &params.ceiling_color);
	wall_bounds(ray, &params.draw_start, &params.draw_end, &line_height);
	wallX = calculate_wall_x(game, ray);
	params.texture = select_texture(game, ray);
	params.tex = init_tex_rendering(params.texture, wallX,
										line_height, params.draw_start);
	draw_column(game, x, &params);
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
