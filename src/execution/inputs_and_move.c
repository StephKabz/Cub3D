/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inputs_and_move.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkabang <stkabang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 14:41:15 by stkabang          #+#    #+#             */
/*   Updated: 2025/12/13 20:33:35 by stkabang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubd3.h"

int	key_press(int keycode, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (keycode == KEY_W)
		game->keys.w = 1;
	else if (keycode == KEY_A)
		game->keys.a = 1;
	else if (keycode == KEY_S)
		game->keys.s = 1;
	else if (keycode == KEY_D)
		game->keys.d = 1;
	else if (keycode == KEY_LEFT)
		game->keys.left = 1;
	else if (keycode == KEY_RIGHT)
		game->keys.right = 1;
	else if (keycode == KEY_ESC)
		game->keys.esc = 1;
	return (0);
}

int	key_release(int keycode, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (keycode == KEY_W)
		game->keys.w = 0;
	else if (keycode == KEY_A)
		game->keys.a = 0;
	else if (keycode == KEY_S)
		game->keys.s = 0;
	else if (keycode == KEY_D)
		game->keys.d = 0;
	else if (keycode == KEY_LEFT)
		game->keys.left = 0;
	else if (keycode == KEY_RIGHT)
		game->keys.right = 0;
	else if (keycode == KEY_ESC)
		game->keys.esc = 0;
	return (0);
}

void	handle_movement(t_game *game)
{
	if (game->keys.w == 1)
		move_forward(game);
	if (game->keys.s == 1)
		move_backward(game);
	if (game->keys.d == 1)
		move_right(game);
	if (game->keys.a == 1)
		move_left(game);
}

void	handle_rotation(t_game *game)
{
	float	old_dir_x;
	float	old_plane_x;
	float	angle;

	if (game->keys.left == 1)
	{
		angle = -ROT_SPEED;
		old_dir_x = game->player.dir_x;
		game->player.dir_x = old_dir_x * cos(angle) - game->player.dir_y * sin(angle);
		game->player.dir_y = old_dir_x * sin(angle) + game->player.dir_y * cos(angle);
		old_plane_x = game->player.plane_x;
		game->player.plane_x = old_plane_x * cos(angle) - game->player.plane_y * sin(angle);
		game->player.plane_y = old_plane_x * sin(angle) + game->player.plane_y * cos(angle);
	}
	if (game->keys.right == 1)
	{
		angle = ROT_SPEED;
		old_dir_x = game->player.dir_x;
		game->player.dir_x = old_dir_x * cos(angle) - game->player.dir_y * sin(angle);
		game->player.dir_y = old_dir_x * sin(angle) + game->player.dir_y * cos(angle);
		old_plane_x = game->player.plane_x;
		game->player.plane_x = old_plane_x * cos(angle) - game->player.plane_y * sin(angle);
		game->player.plane_y = old_plane_x * sin(angle) + game->player.plane_y * cos(angle);
	}
}
