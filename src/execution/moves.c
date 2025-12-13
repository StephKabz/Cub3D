/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moves.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkabang <stkabang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 15:55:13 by stkabang          #+#    #+#             */
/*   Updated: 2025/12/13 19:26:45 by stkabang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubd3.h"

void	check_wall(t_game *game, float new_x, float new_y)
{
	if (game->scene.map.lines_map[(int)new_y][(int)new_x] != '1')
	{
		game->player.pos_x = new_x;
		game->player.pos_y = new_y;
	}
}

void	move_forward(t_game *game)
{
	float	new_x;
	float	new_y;

	new_x = game->player.pos_x + game->player.dir_x * MOVE_SPEED;
	new_y = game->player.pos_y + game->player.dir_y * MOVE_SPEED;
	check_wall(game, new_x, new_y);
}

void	move_backward(t_game *game)
{
	float	new_x;
	float	new_y;

	new_x = game->player.pos_x - game->player.dir_x * MOVE_SPEED;
	new_y = game->player.pos_y - game->player.dir_y * MOVE_SPEED;
	check_wall(game, new_x, new_y);
}

void	move_right(t_game *game)
{
	float	new_x;
	float	new_y;

	new_x = game->player.pos_x - game->player.dir_y * MOVE_SPEED;
	new_y = game->player.pos_y + game->player.dir_x * MOVE_SPEED;
	check_wall(game, new_x, new_y);
}

void	move_left(t_game *game)
{
	float	new_x;
	float	new_y;

	new_x = game->player.pos_x + game->player.dir_y * MOVE_SPEED;
	new_y = game->player.pos_y - game->player.dir_x * MOVE_SPEED;
	check_wall(game, new_x, new_y);
}
