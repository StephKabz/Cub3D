/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkabang <stkabang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 16:28:03 by stkabang          #+#    #+#             */
/*   Updated: 2025/12/13 15:42:57 by stkabang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubd3.h"

int	game_loop(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (game->keys.esc == 1)
	{
		cleanup_mlx(game);
		cleanup_scene(&game->scene);
		exit(0);
	}
	handle_movement(game);
	handle_rotation(game);
	raycasting(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
	return (0);
}

int	close_window(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	cleanup_mlx(game);
	cleanup_scene(&game->scene);
	exit(0);
}
