/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inputs_and_move.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkabang <stkabang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 14:41:15 by stkabang          #+#    #+#             */
/*   Updated: 2025/12/10 16:27:18 by stkabang         ###   ########.fr       */
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
	float	newX;
	float	newY;

	if (game->keys.w == 1)
	{
		newX = game->player.posX + game->player.dirX * MOVE_SPEED;
		newY = game->player.posY + game->player.dirY * MOVE_SPEED;
		if (game->scene.map.lines_map[(int)newY][(int)newX] != '1')
		{
			game->player.posX = newX;
			game->player.posY = newY;
		}
	}
	if (game->keys.s == 1)
	{
		newX = game->player.posX - game->player.dirX * MOVE_SPEED;
		newY = game->player.posY - game->player.dirY * MOVE_SPEED;
		if (game->scene.map.lines_map[(int)newY][(int)newX] != '1')
		{
			game->player.posX = newX;
			game->player.posY = newY;
		}
	}
	if (game->keys.a == 1)
	{
		newX = game->player.posX - game->player.dirY * MOVE_SPEED;
		newY = game->player.posY + game->player.dirX * MOVE_SPEED;
		if (game->scene.map.lines_map[(int)newY][(int)newX] != '1')
		{
			game->player.posX = newX;
			game->player.posY = newY;
		}
	}
	if (game->keys.d == 1)
	{
		newX = game->player.posX + game->player.dirY * MOVE_SPEED;
		newY = game->player.posY - game->player.dirX * MOVE_SPEED;
		if (game->scene.map.lines_map[(int)newY][(int)newX] != '1')
		{
			game->player.posX = newX;
			game->player.posY = newY;
		}
	}
}

void	handle_rotation(t_game *game)
{
	float	oldDirX;
	float	oldPlaneX;
	float	angle;

	if (game->keys.left == 1)
	{
		angle = -ROT_SPEED;
		oldDirX = game->player.dirX;
		game->player.dirX = oldDirX * cos(angle) - game->player.dirY * sin(angle);
		game->player.dirY = oldDirX * sin(angle) + game->player.dirY * cos(angle);
		oldPlaneX = game->player.planeX;
		game->player.planeX = oldPlaneX * cos(angle) - game->player.planeY * sin(angle);
		game->player.planeY = oldPlaneX * sin(angle) + game->player.planeY * cos(angle);
	}
	if (game->keys.right == 1)
	{
		angle = ROT_SPEED;
		oldDirX = game->player.dirX;
		game->player.dirX = oldDirX * cos(angle) - game->player.dirY * sin(angle);
		game->player.dirY = oldDirX * sin(angle) + game->player.dirY * cos(angle);
		oldPlaneX = game->player.planeX;
		game->player.planeX = oldPlaneX * cos(angle) - game->player.planeY * sin(angle);
		game->player.planeY = oldPlaneX * sin(angle) + game->player.planeY * cos(angle);
	}
}
