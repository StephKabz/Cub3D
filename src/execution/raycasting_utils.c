/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkabang <stkabang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 19:07:58 by stkabang          #+#    #+#             */
/*   Updated: 2025/12/13 19:14:22 by stkabang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubd3.h"

double	ft_floor(double nb)
{
	if (nb >= 0)
		return ((double)(int)nb);
	if (nb != ((int)nb))
		return ((double)(int)nb - 1);
	return ((double)(int)nb);
}

double	ft_abs_double(double nb)
{
	if (nb < 0)
		return (-nb);
	return (nb);
}
