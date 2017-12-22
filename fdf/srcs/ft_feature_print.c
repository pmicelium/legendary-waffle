/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_feature_print.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmiceli <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/19 15:30:51 by pmiceli           #+#    #+#             */
/*   Updated: 2017/12/22 17:20:29 by pmiceli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

static int	ft_set_color(t_fdf fdf, char color)
{
	int mlx_color;

	if (color == 'R')
		mlx_color = 0x00FF0000;
	if (color == 'G')
		mlx_color = 0x0000FF00;
	if (color == 'B')
		mlx_color = 0x000000FF;
	if (color == 'L')
		mlx_color = fdf.pos.low_color;
	if (color == 'H')
		mlx_color = fdf.pos.high_color;
	return (mlx_color);
}

static void	ft_feature_key(t_fdf fdf)
{
	mlx_string_put(fdf.mlx, fdf.win1, 25, 25, 0x00FFFFFF,  "Features :");
	mlx_string_put(fdf.mlx, fdf.win1, 25, 50, 0x00c1c1c1,  "Zoom           : 0");
	mlx_string_put(fdf.mlx, fdf.win1, 25, 70, 0x00c1c1c1,  "Zoom out       : 9");
	mlx_string_put(fdf.mlx, fdf.win1, 25, 90, 0x00c1c1c1,  "Elevation +    : +");
	mlx_string_put(fdf.mlx, fdf.win1, 25, 110, 0x00c1c1c1, "Elevation -    : -");
	mlx_string_put(fdf.mlx, fdf.win1, 25, 130, 0x00c1c1c1, "Mouve image    : wasd");
	mlx_string_put(fdf.mlx, fdf.win1, 25, 150, 0x00c1c1c1, "Reset image    : space");
	mlx_string_put(fdf.mlx, fdf.win1, 25, 170, 0x00c1c1c1, "hidden surface : c");
}

static void	ft_feature_var(t_fdf fdf)
{
	mlx_string_put(fdf.mlx, fdf.win1, X_WIN_1 -100, 25, 0x00c1c1c1, "FPS :");
	mlx_string_put(fdf.mlx, fdf.win1, X_WIN_1 -110, 45, 0x00c1c1c1, ft_strjoin("Zoom : x", ft_itoa(fdf.key.zoom)));
	mlx_string_put(fdf.mlx, fdf.win1, X_WIN_1 -160, 65, 0x00c1c1c1, ft_strjoin("Elevation : ", ft_itoa(fdf.key.elev)));
	mlx_string_put(fdf.mlx, fdf.win1, X_WIN_1 -220, 85, 0x00c1c1c1, "hidden surfaces : ");
	if (fdf.key.face == 1)
		mlx_string_put(fdf.mlx, fdf.win1, X_WIN_1 -40, 85, 0x00FF0000, "off");
	else
		mlx_string_put(fdf.mlx, fdf.win1, X_WIN_1 -40, 85, 0x0000FF00, "on");

}

void	ft_feature_print(t_fdf fdf)
{
	ft_feature_key(fdf);
	ft_feature_var(fdf);
}
