/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_algo_xiaolin_wu.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmiceli <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/18 22:55:02 by pmiceli           #+#    #+#             */
/*   Updated: 2018/01/19 06:24:40 by pmiceli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"
#include "../includes/xiaolin_wu.h"

int				set_brightness(int color, float br)
{
	int		a;
	int		r;
	int		g;
	int		b;
	int		color_ret;

	a = 255 - 255 * br;
	r = (color >> 16) & 0xFF;
	g = (color >> 8) & 0xFF;
	b = (color) & 0xFF;
	color_ret = (a & 0xFF) << 24 |
		(r & 0xFF) << 16 | (g & 0xFF) << 8 | (b & 0xFF);
	return (color_ret);
}

int		check_print(int x, int y)
{
	if ((y * X_WIN_1 + x) < X_WIN_1 * Y_WIN_1
			&& y > 0 && x > 0
			&& y < Y_WIN_1 && x < X_WIN_1)
		return (1);
	return (0);
}

static t_wu		set_wu(t_wu wu, t_put put, int i)
{
	if (i == 1)
	{
		wu.gradient = (double)put.dy / (double)put.dx;
		wu.xend = ROUND_(put.x0);
		wu.yend = put.y0 + wu.gradient * (wu.xend - put.x0);
		wu.xgap = rfpart_(put.x0 + 0.5);
		wu.xpx11 = wu.xend;
		wu.ypx11 = ipart_(wu.yend);
	}
	if (i == 2)
	{
		wu.intery = wu.yend + wu.gradient;
		wu.xend = round_(put.x1);
		wu.yend = put.y1 + wu.gradient * (wu.xend - put.x1);
		wu.xgap = fpart_(put.x1 + 0.5);
		wu.xpx12 = wu.xend;
		wu.ypx12 = ipart_(wu.yend);
	}
	return (wu);
}

static void		xiaolin_wu_2(t_fdf fdf, t_put put, t_wu wu)
{
	int			x;
	double		steap;
	int			i;

	i = 0;
	steap = sqrt(pow((put.x1 - put.x0), 2) + pow((put.y1 - put.y0), 2));
	wu = set_wu(wu, put, 1);
	if (check_print(wu.xpx11, wu.ypx11 + 1) == 1)
	{
		print_xiao(wu.xpx11, wu.ypx11, rfpart_(wu.yend) * wu.xgap,
				fdf, put.color);
		print_xiao(wu.xpx11, wu.ypx11 + 1, fpart_(wu.yend) * wu.xgap,
				fdf, put.color);
	}
	wu = set_wu(wu, put, 2);
	if (check_print(wu.xpx12, wu.ypx12 + 1) == 1)
	{
		print_xiao(wu.xpx12, wu.ypx12, rfpart_(wu.yend) * wu.xgap,
				fdf, put.color1);
		print_xiao(wu.xpx12, wu.ypx12 + 1, fpart_(wu.yend) * wu.xgap,
				fdf, put.color1);
	}
	x = wu.xpx11;
	while (x <= (wu.xpx12 - 1))
	{
		if (check_print(x, ipart_(wu.intery + 1)) == 1)
		{
			print_xiao(x, ipart_(wu.intery), rfpart_(wu.intery), fdf,
					ft_degraded(fdf.pos.degrad, i / steap));
			print_xiao(x, ipart_(wu.intery) + 1, fpart_(wu.intery), fdf,
					ft_degraded(fdf.pos.degrad, i++ / steap));
		}
		wu.intery += wu.gradient;
		x++;
	}
}

void			xiaolin_wu(t_fdf fdf, t_put put)
{
	t_wu		wu;

	wu.xend = 0;
	put.dx = (double)put.x1 - (double)put.x0;
	put.dy = (double)put.y1 - (double)put.y0;

	if (ft_abs(put.dx) > ft_abs(put.dy))
	{
		if (put.x1 < put.x0)
		{
			ft_swap_nbr(&put.x1, &put.x0);
			ft_swap_nbr(&put.y1, &put.y0);
			ft_swap_nbr(&put.color, &put.color1);
		}
		fdf.pos.degrad = set_degrad(put, fdf.pos.degrad);
		xiaolin_wu_2(fdf, put, wu);
	}
	else
	{
		if (put.y1 < put.y0)
		{
			ft_swap_nbr(&put.x1, &put.x0);
			ft_swap_nbr(&put.y1, &put.y0);
			ft_swap_nbr(&put.color, &put.color1);
		}
		fdf.pos.degrad = set_degrad(put, fdf.pos.degrad);
		xiaolin_wu_3(fdf, put, wu);
	}
}