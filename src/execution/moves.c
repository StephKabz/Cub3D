/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moves.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkabang <stkabang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 15:55:13 by stkabang          #+#    #+#             */
/*   Updated: 2025/12/13 16:36:07 by stkabang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubd3.h"

void	check_wall(t_game *game, float newX, float newY)
{
	if (game->scene.map.lines_map[(int)newY][(int)newX] != '1')
	{
		game->player.pos_x = newX;
		game->player.pos_y = newY;
	}
}

void	move_forward(t_game *game)
{
	float	newX;
	float	newY;

	newX = game->player.pos_x + game->player.dir_x * MOVE_SPEED;
	newY = game->player.pos_y + game->player.dir_y * MOVE_SPEED;
	check_wall(game, newX, newY);
}

void	move_backward(t_game *game)
{
	float	newX;
	float	newY;

	newX = game->player.pos_x - game->player.dir_x * MOVE_SPEED;
	newY = game->player.pos_y - game->player.dir_y * MOVE_SPEED;
	check_wall(game, newX, newY);
}

void	move_right(t_game *game)
{
	float	newX;
	float	newY;

	newX = game->player.pos_x - game->player.dir_y * MOVE_SPEED;
	newY = game->player.pos_y + game->player.dir_x * MOVE_SPEED;
	check_wall(game, newX, newY);
}

void	move_left(t_game *game)
{
	float	newX;
	float	newY;

	newX = game->player.pos_x + game->player.dir_y * MOVE_SPEED;
	newY = game->player.pos_y - game->player.dir_x * MOVE_SPEED;
	check_wall(game, newX, newY);
}

