/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_op.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 13:56:45 by tamigore          #+#    #+#             */
/*   Updated: 2021/04/19 12:47:27 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmath.h"
#include "../../../includes/libft.h"
#include <stdio.h>

void			mat4_x_mat4(double **a, double **b, double **mat)
{
	int		i;
	int		j;
	int		k;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			k = 0;
			while (k < 4)
			{
				mat[i][j] = a[i][k] * b[k][j] + mat[i][j];
				k++;
			}
			j++;
		}
		i++;
	}
}

t_v3			vec3_x_mat4(t_v3 vec, double **m)
{
	t_v3	res;

	res.x = m[0][0] * vec.x + m[0][1] * vec.y + m[0][2] * vec.z + m[0][3];
	res.y = m[1][0] * vec.x + m[1][1] * vec.y + m[1][2] * vec.z + m[1][3];
	res.z = m[2][0] * vec.x + m[2][1] * vec.y + m[2][2] * vec.z + m[2][3];
	return (res);
}

static void		invers_start(double **inv, double **m)
{
	inv[0][0] = m[1][1] * m[2][2] * m[3][3] - m[1][1] * m[2][3] * m[3][2] -
				m[2][1] * m[1][2] * m[3][3] + m[2][1] * m[1][3] * m[3][2] +
				m[3][1] * m[1][2] * m[2][3] - m[3][1] * m[1][3] * m[2][2];
	inv[1][0] = -m[1][0] * m[2][2] * m[3][3] + m[1][0] * m[2][3] * m[3][2] +
				m[2][0] * m[1][2] * m[3][3] - m[2][0] * m[1][3] * m[3][2] -
				m[3][0] * m[1][2] * m[2][3] + m[3][0] * m[1][3] * m[2][2];
	inv[2][0] = m[1][0] * m[2][1] * m[3][3] - m[1][0] * m[2][3] * m[3][1] -
				m[2][0] * m[1][1] * m[3][3] + m[2][0] * m[1][3] * m[3][1] +
				m[3][0] * m[1][1] * m[2][3] - m[3][0] * m[1][3] * m[2][1];
	inv[3][0] = -m[1][0] * m[2][1] * m[3][2] + m[1][0] * m[2][2] * m[3][1] +
				m[2][0] * m[1][1] * m[3][2] - m[2][0] * m[1][2] * m[3][1] -
				m[3][0] * m[1][1] * m[2][2] + m[3][0] * m[1][2] * m[2][1];
	inv[0][1] = -m[0][1] * m[2][2] * m[3][3] + m[0][1] * m[2][3] * m[3][2] +
				m[2][1] * m[0][2] * m[3][3] - m[2][1] * m[0][3] * m[3][2] -
				m[3][1] * m[0][2] * m[2][3] + m[3][1] * m[0][3] * m[2][2];
	inv[1][1] = m[0][0] * m[2][2] * m[3][3] - m[0][0] * m[2][3] * m[3][2] -
				m[2][0] * m[0][2] * m[3][3] + m[2][0] * m[0][3] * m[3][2] +
				m[3][0] * m[0][2] * m[2][3] - m[3][0] * m[0][3] * m[2][2];
	inv[2][1] = -m[0][0] * m[2][1] * m[3][3] + m[0][0] * m[2][3] * m[3][1] +
				m[2][0] * m[0][1] * m[3][3] - m[2][0] * m[0][3] * m[3][1] -
				m[3][0] * m[0][1] * m[2][3] + m[3][0] * m[0][3] * m[2][1];
	inv[3][1] = m[0][0] * m[2][1] * m[3][2] - m[0][0] * m[2][2] * m[3][1] -
				m[2][0] * m[0][1] * m[3][2] + m[2][0] * m[0][2] * m[3][1] +
				m[3][0] * m[0][1] * m[2][2] - m[3][0] * m[0][2] * m[2][1];
}

static void		invers_end(double **inv, double **m)
{
	inv[0][2] = m[0][1] * m[1][2] * m[3][3] - m[0][1] * m[1][3] * m[3][2] -
				m[1][1] * m[0][2] * m[3][3] + m[1][1] * m[0][3] * m[3][2] +
				m[3][1] * m[0][2] * m[1][3] - m[3][1] * m[0][3] * m[1][2];
	inv[1][2] = -m[0][0] * m[1][2] * m[3][3] + m[0][0] * m[1][3] * m[3][2] +
				m[1][0] * m[0][2] * m[3][3] - m[1][0] * m[0][3] * m[3][2] -
				m[3][0] * m[0][2] * m[1][3] + m[3][0] * m[0][3] * m[1][2];
	inv[2][2] = m[0][0] * m[1][1] * m[3][3] - m[0][0] * m[1][3] * m[3][1] -
				m[1][0] * m[0][1] * m[3][3] + m[1][0] * m[0][3] * m[3][1] +
				m[3][0] * m[0][1] * m[1][3] - m[3][0] * m[0][3] * m[1][1];
	inv[3][2] = -m[0][0] * m[1][1] * m[3][2] + m[0][0] * m[1][2] * m[3][1] +
				m[1][0] * m[0][1] * m[3][2] - m[1][0] * m[0][2] * m[3][1] -
				m[3][0] * m[0][1] * m[1][2] + m[3][0] * m[0][2] * m[1][1];
	inv[0][3] = -m[0][1] * m[1][2] * m[2][3] + m[0][1] * m[1][3] * m[2][2] +
				m[1][1] * m[0][2] * m[2][3] - m[1][1] * m[0][3] * m[2][2] -
				m[2][1] * m[0][2] * m[1][3] + m[2][1] * m[0][3] * m[1][2];
	inv[1][3] = m[0][0] * m[1][2] * m[2][3] - m[0][0] * m[1][3] * m[2][2] -
				m[1][0] * m[0][2] * m[2][3] + m[1][0] * m[0][3] * m[2][2] +
				m[2][0] * m[0][2] * m[1][3] - m[2][0] * m[0][3] * m[1][2];
	inv[2][3] = -m[0][0] * m[1][1] * m[2][3] + m[0][0] * m[1][3] * m[2][1] +
				m[1][0] * m[0][1] * m[2][3] - m[1][0] * m[0][3] * m[2][1] -
				m[2][0] * m[0][1] * m[1][3] + m[2][0] * m[0][3] * m[1][1];
	inv[3][3] = m[0][0] * m[1][1] * m[2][2] - m[0][0] * m[1][2] * m[2][1] -
				m[1][0] * m[0][1] * m[2][2] + m[1][0] * m[0][2] * m[2][1] +
				m[2][0] * m[0][1] * m[1][2] - m[2][0] * m[0][2] * m[1][1];
}

int				mat_invers(double **m, double **invert)
{
	double	**inv;
	double	det;
	int		i;
	int		j;

	inv = mat_init();
	invers_start(inv, m);
	invers_end(inv, m);
	det = m[0][0] * inv[0][0] + m[0][1] * inv[1][0] + m[0][2] * inv[2][0] +
		m[0][3] * inv[3][0];
	if (det == 0)
		return (0);
	det = 1.0 / det;
	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			invert[i][j] = inv[i][j] * det;
			j++;
		}
		i++;
	}
	return (1);
}
