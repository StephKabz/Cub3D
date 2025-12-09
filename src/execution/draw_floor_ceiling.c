/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_floor_ceiling.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkabang <stkabang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 15:19:23 by stkabang          #+#    #+#             */
/*   Updated: 2025/12/08 15:48:11 by stkabang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubd3.h"

void	draw_floor_ceiling(t_game *game)
{
	int	color;
	int	floor_color;
	int	ceiling_color;
	int	x;
	int	y;

	floor_color = (game->scene.fc.floor.red << 16)
				| (game->scene.fc.floor.green << 8)
				| (game->scene.fc.floor.blue);
	ceiling_color = (game->scene.fc.ceiling.red << 16)
				| (game->scene.fc.ceiling.green << 8)
				| (game->scene.fc.ceiling.blue);
	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			if (y < HEIGHT / 2)
				color = ceiling_color;
			else
				color = floor_color;
			put_pixel(game, x, y, color);
			x++;
		}
		y++;
	}
}
