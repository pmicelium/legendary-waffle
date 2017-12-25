/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_place.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmiceli <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/25 17:29:13 by pmiceli           #+#    #+#             */
/*   Updated: 2017/12/25 18:20:10 by pmiceli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

//
//
//
//  FAIRE DU CLIPING de la taille de la fenetre pour eviter le crach random
//
//
//
//  pour les faces caches faire un parse pendant que je draw pour mettre en noir chque pixel
//  qu'il y a dans le carre de dim i / i + 1 / i + pos.x / i + pos.x + 1
//
//
//

static void	ft_link_point(t_fdf fdf, t_pos pos)
{
	t_put	put;
	int		i;
	int		j;
	int		color;

	i = 0;
	j = 0;
	while (j < pos.y)
	{
		while (i < pos.x * j)
		{
			put.x0 = pos.placex[i];
			put.y0 = pos.placey[i];
			if (i + 1 != pos.x * j)
			{
				put.x1 = pos.placex[i + 1];
				put.y1 = pos.placey[i + 1];
				color = pos.elev[i] == 1 && pos.elev[i + 1] == 1 ? pos.high_color : pos.low_color;
				if (pos.elev [i] != 1 && pos.elev[i + 1] == 1)
					bresenham_line(fdf, put, color, 1);
				else if (pos.elev[i] == 1 && pos.elev[i + 1] != 1)
					bresenham_line(fdf, put, color, 2);
				else
					bresenham_line(fdf, put, color, 0);
			}
			if (j != pos.y - 1)
			{
				put.x1 = pos.placex[i + pos.x];
				put.y1 = pos.placey[i + pos.x];
				color = pos.elev[i] == 1 && pos.elev[i + pos.x] == 1 ? pos.high_color : pos.low_color;
				if (pos.elev [i] != 1 && pos.elev[i + pos.x] == 1)
					bresenham_line(fdf, put, color, 1);
				else if  (pos.elev[i] == 1 && pos.elev[i + pos.x] != 1)
					bresenham_line(fdf, put, color, 2);
				else
					bresenham_line(fdf, put, color, 0);
			}
			i++;
		}
		j++;
	}
}

static t_cal	ft_calculator(t_fdf *fdf, int x, int y)
{
	t_cal cal;

	cal.x_len = x * fdf->key.zoom * fdf->key.x_deriv;
	cal.y_len = y * fdf->key.zoom * fdf->key.y_deriv;
	cal.base_x_len = x * fdf->key.zoom;
	cal.base_y_len = y * fdf->key.zoom;
	cal.y_trans = sqrt((pow(cal.base_y_len, 2) - pow(cal.y_len, 2)));
	cal.x_trans = sqrt((pow(cal.base_x_len, 2) - pow(cal.x_len, 2)));
	return (cal);
}

static void	ft_place_point(t_fdf *fdf, t_pos *pos, t_key key)
{
	int x;
	int y;
	int i;
	int tmp_px;
	int tmp_py;
	int tmp_pz;

	i = 0;
	y = 0;
	if (!(pos->placex = (int*)malloc(sizeof(int) * (pos->x * (pos->y - 1)))))
		exit(1);
	if (!(pos->placey = (int*)malloc(sizeof(int) * (pos->x * (pos->y - 1)))))
		exit(1);
	if (!(pos->placez = (int*)malloc(sizeof(int) * (pos->x * (pos->y - 1)))))
		exit(1);
	if (!(pos->elev = (int*)malloc(sizeof(int) * (pos->x * (pos->y - 1)))))
		exit(1);
	while (y < pos->y - 1)
	{
		x = 0;
		while (x < pos->x)
		{
			pos->cal = ft_calculator(fdf, x, y);
			pos->elev[i] = pos->z[y][x] != 0 ? 1 : 0;

			pos->placex[i] = pos->cal.base_x_len;
			pos->placey[i] = pos->cal.base_y_len; // mettre les modif sur z ici a la place du key.deriv ou faire un placez;
			pos->placez[i] = pos->z[y][x] * key.elev;

			tmp_px = pos->placex[i];
			tmp_py = pos->placey[i];
			tmp_pz = pos->placez[i];

			pos->placex[i] = tmp_px * cos(-fdf->key.rot_z * M_PI / 180) - tmp_py * sin(-fdf->key.rot_z * M_PI / 180);
			pos->placey[i] = tmp_py * cos(-fdf->key.rot_z * M_PI / 180) + tmp_px * sin(-fdf->key.rot_z * M_PI / 180);

			tmp_px = pos->placex[i];
			tmp_py = pos->placey[i];

			pos->placey[i] = tmp_py * cos(fdf->key.rot_x * M_PI / 180) - tmp_pz * sin(fdf->key.rot_x * M_PI / 180);
			pos->placez[i] = tmp_py * sin(fdf->key.rot_x * M_PI / 180) + tmp_pz * cos(fdf->key.rot_x * M_PI / 180);

			tmp_py = pos->placey[i];
			tmp_pz = pos->placez[i];

			pos->placex[i] = tmp_px * cos(fdf->key.rot_y * M_PI / 180) + tmp_pz * sin(fdf->key.rot_y * M_PI / 180);

			pos->placex[i] += (400 + fdf->key.zoom / 10) + key.a;
			pos->placey[i] += (400 + fdf->key.zoom / 10) + key.w;
			i++;
			x++;
		}
		y++;
	}
	ft_link_point(*fdf, *pos);
}

void	ft_place(t_fdf fdf, t_pos pos, t_key key)
{
	clock_t time;
	static clock_t start;
	static unsigned int fps;
	static unsigned int tmp = 0;

	time = clock();
	ft_place_point(&fdf, &pos, key);
	mlx_put_image_to_window(fdf.mlx, fdf.win1, fdf.img, 0, 0);
	if (key.hud == 1)
	{
		ft_feature_print(fdf);

		fps++;
		time = clock() - time;
		start += time;
		if (((float)start/CLOCKS_PER_SEC) >= 0.25)
		{
			mlx_string_put(fdf.mlx, fdf.win1, X_WIN_1 -40, 25, 0x00c1c1c1, ft_itoa(fps * 4));
			tmp = fps * 4;
			fps = 0;
			start = 0;
		}
		else
			mlx_string_put(fdf.mlx, fdf.win1, X_WIN_1 -40, 25, 0x00c1c1c1, ft_itoa(tmp));
	}
}
