/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mlx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkabang <stkabang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 16:58:22 by kingstephan       #+#    #+#             */
/*   Updated: 2025/12/13 16:52:32 by stkabang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubd3.h"

int	init_mlx(t_game *game, char *title)
{
	game->mlx = mlx_init();
	if (!game->mlx)
		return (0);
	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, title);
	if (!game->win)
		return (0);
	game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	if (!game->img)
		return (0);
	game->img_data = mlx_get_data_addr(game->img, &game->bpp, &game->line_len, &game->endian);
	if (!game->img_data)
		return (0);
	return (1);
}

void	put_pixel(t_game *game, int x, int y, int color)
{
	int	offset;

	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return ;
	offset = y * game->line_len + x * (game->bpp / 8);
	*(int *)(game->img_data + offset) = color;
}

void	init_spawn_dir_and_pos(t_player *player, t_spawn *spawn)
{
	if (spawn->dir == 'N')
	{
		player->dir_x = 0;
		player->dir_y = -1;
	}
	else if (spawn->dir == 'S')
	{
		player->dir_x = 0;
		player->dir_y = 1;
	}
	else if (spawn->dir == 'E')
	{
		player->dir_x = 1;
		player->dir_y = 0;
	}
	else if (spawn->dir == 'W')
	{
		player->dir_x = -1;
		player->dir_y = 0;
	}
	player->pos_x = spawn->col + 0.5;
	player->pos_y = spawn->row + 0.5;
	player->plane_x = -player->dir_y * 0.66;
	player->plane_y = player->dir_x * 0.66;
}

static int	load_texture(t_textures_img *texture, void *mlx, char *path)
{
	texture->img = mlx_xpm_file_to_image(mlx, path, &texture->width, &texture->height);
	if (!texture->img)
		return (0);
	texture->img_data = mlx_get_data_addr(texture->img, &texture->bpp, &texture->line_len, &texture->endian);
	if (!texture->img_data)
		return (0);
	return (1);
}

int	load_all_textures(t_game *game)
{
	if (!load_texture(&game->textures.north, game->mlx,
			game->scene.textures.no))
		return (0);
	if (!load_texture(&game->textures.south, game->mlx,
			game->scene.textures.so))
		return (0);
	if (!load_texture(&game->textures.west, game->mlx,
			game->scene.textures.we))
		return (0);
	if (!load_texture(&game->textures.east, game->mlx,
			game->scene.textures.ea))
		return (0);
	return (1);
}


