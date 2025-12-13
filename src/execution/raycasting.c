/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkabang <stkabang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 16:25:10 by stkabang          #+#    #+#             */
/*   Updated: 2025/12/13 19:10:45 by stkabang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubd3.h"

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
