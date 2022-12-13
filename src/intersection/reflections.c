/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflections.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 16:59:06 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/05 19:33:20 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_color	calculate_reflected_color(t_intersections *arr, t_scene *scene, t_ray *ray, int remaining, int light_idx)
{
	t_intersect		*itx;
	t_color			final_color;
	t_color			light_color;

	ft_bzero(&final_color, sizeof(t_color));
	itx = hit(arr);
	if (itx != NULL)
	{
		prepare_computations(itx, ray);
		// while (light_idx < scene->count.light_count)
		// {
		light_color = lighting(itx, scene, light_idx);
		t_color	reflected = reflected_color(scene, itx, remaining - 1, light_idx);
		add_colors(&final_color, &reflected, &light_color);
		add_colors(&final_color, &final_color, &light_color);
		// light_idx++;
		// }
		// itx->shape->mlx_color = create_mlx_color(&final_color);
	}
	return (final_color);
}

t_color	reflected_color(t_scene *scene, t_intersect *intersection, int remaining, int light_idx)
{
	t_color			color;
	t_ray			ray;
	t_intersections	arr;
	unsigned int	shape_idx;

	if (intersection->shape->reflectiveness == 0 || remaining == 0)
	{
		ft_bzero(&color, sizeof(t_color));
		return (color);
	}
	ft_memcpy(&ray.origin, &intersection->over_point, sizeof(t_vector));
	ft_memcpy(&ray.direction, &intersection->reflect_vec, sizeof(t_vector));
	shape_idx = -1;
	arr.count = 0;
	while (++shape_idx < scene->count.shape_count)
		intersect(&scene->shapes[shape_idx], &ray, &arr);
	// calculate_lighting(&arr, worker, &ray, (y * worker->scene->render_w \
	// 	+ x) * worker->scene->mlx->bytes_per_pixel);
	t_color reflected = calculate_reflected_color(&arr, scene, &ray, remaining, light_idx);
	mult_color(&reflected, &reflected, intersection->shape->reflectiveness);
	return (reflected);
	// return color * comps.object.material.reflective
}