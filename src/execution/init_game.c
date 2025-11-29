/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkabang <stkabang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 20:05:44 by stkabang          #+#    #+#             */
/*   Updated: 2025/11/29 20:28:22 by stkabang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubd3.h"

int	init_game(t_game *game)
{
	if (!init_mlx(game, "cub3D"))
		return (0);
	if (!load_all_textures(game))
		return (0);
	init_spawn_dir_and_pos(&game->player, &game->scene.spawn);
	return (1);
}

