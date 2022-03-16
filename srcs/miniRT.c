/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 13:28:29 by tamigore          #+#    #+#             */
/*   Updated: 2022/03/16 14:11:24 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
** usage : secure file
** ATTENTION |.rt|
*/
static int	usage(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == '.')
		{
			if (!ft_strcmp(".rt", &str[i]))
				return (1);
		}
		i++;
	}
	return (0);
}

static void			put_pixel_to_image(t_env *env, t_v3 color, int x, int y)
{
	int				i;

	i = (x * PIXEL_LEN + env->img->size_line * y);
	color = rescale_vec(color, 0, MAX_RGB);
	env->img->pixels[i + TRANS] = (unsigned int)0;
	env->img->pixels[i + RED] = (unsigned int)color.x;
	env->img->pixels[i + GREEN] = (unsigned int)color.y;
	env->img->pixels[i + BLUE] = (unsigned int)color.z;
}

// static void			shade(t_env *env, t_ray *ray)
// {
// 	t_obj			*hit_obj;

// 	hit_obj = trace_objs(env->obj, ray);
// 	if (hit_obj)
// 	{
// 		get_obj_color(hit_obj, ray);
// 		ray->hit = v_add(ray->pos, v_multi(ray->t, ray->dir));
// 		get_obj_normal(hit_obj, ray);
// 		ray->color = trace_ray_to_light(env, ray); // creat lights
// 		// if (ray->hit.x || ray->hit.z || ray->hit.y)
// 		// {
// 		// 	ray->color.x = 255;
// 		// 	ray->color.y = 255;
// 		// 	ray->color.z = 255;
// 		// }
// 	}
// }
int			inter(t_sph *sph, t_ray *ray, double *t)
{
	double	t0 = 0.0;
	double	t1 = 0.0;
	double delta;
	delta = (pow(v_dot(ray->dir, v_sub(ray->pos, sph->pos)), 2)) - (pow(v_len(v_sub(ray->pos, sph->pos)), 2) - (sph->r * sph->r));
	//printf("DELTA = %f\n", delta);
	t0 = - (v_dot(ray->dir, v_sub(ray->pos, sph->pos))) + sqrt(delta);
	t1 = - (v_dot(ray->dir, v_sub(ray->pos, sph->pos))) - sqrt(delta);

	if (delta < 0)
		return (0);
	else if (delta == 0)
	{
		if (t0 < t1 && t0 > 0)
			*t = t0;
		else if (t1 < t0 && t1 > 0)
			*t = t1;
		return (1);
	}
	else if (delta > 0)
	{
		if (t1 > 0 || t0 > 0)
		{
			if (t1 <= t0)
				*t = t1;
			else
				*t = t0;
			return (1);
		}
	}
	return (0);
}


void	img_calc(t_env *env)
{
	t_img *img;
	int x;
	int y;
	double xx;
	double yy;
	double t;
	t_v3 color;
	t_ray ray;

	color.x = 250;
	color.y = 0;
	color.z = 0;

	x = 0;
	y = 0;
	init_ray(&ray);
	img = env->img;
	//printf("Image : %d / %d\n", img->size_x, img->size_y);
	//printf("CAM : \n	pos : [%f, %f, %f]\n	dir : [%f, %f, %f]\n", env->cam->pos.x,env->cam->pos.y,env->cam->pos.z,env->cam->dir.x,env->cam->dir.y,env->cam->dir.z);
    double angle = tan(env->cam->fov / 2 * M_PI / 180); 
	while (y < img->size_y)
	{
		while (x < img->size_x)
		{
			xx = (double)(2 * ((x + 0.5) * (1/(double)img->size_x)) - 1) * angle;
            yy = (double)(1 - 2 * ((y + 0.5) * (1/(double)img->size_y))) * angle; 
			reset_ray(&ray);
			set_ray(&ray, env->cam->pos, v_init(xx,yy, 1), INFINITY); //aspect ration
			//set_ray(&ray, env->cam->pos, v_init(xx,yy, 1 - fabs((yy + xx) * cos(env->cam->fov))), INFINITY); //aspect ration
			ray.dir = v_norm(ray.dir);
			// if (x > (3*(img->size_x / 4)))
			// 	ray.dir.z = ray.dir.z-(ray.dir.z / 100);
			if (inter((t_sph *)env->obj->data, &ray, &t))
				put_pixel_to_image(env, v_init(255,255,255), x, y);
			else
				put_pixel_to_image(env, v_init(0,0,0), x, y);
			//;
			printf("dir : [%f, %f, %f]\n", ray.dir.x,ray.dir.y,ray.dir.z);
			x++;
		}
		x = 0;
		y++;
	}
}

/*
** main : check | pars & initialize | compute ray | graphic interface
*/
int			main(int ac, char **av)
{
	t_env	*env;

	if (ac != 2 || !av || !usage(av[1])) // first check
	{
			ft_putstr_fd("Error: Usage: minirt: ./miniRT <scene.rt>\n", 2);
			exit(0);
	}
	printf("arg : %s\n", av[1]);
	env = init_env(); // second check && init
	pars_sceen(av[1], env); // parsing
	env->img = init_img(env);
	img_calc(env);
	graphic_loop(env);
	return (END);
}
