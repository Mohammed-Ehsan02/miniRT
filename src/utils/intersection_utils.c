/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 17:23:32 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/05 14:39:32 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	prepare_computations(t_intersection *intersection,
			t_ray *ray)
{
	ray_position(&intersection->point, ray, intersection->time);
	intersection->normal = normal_at(intersection->shape, &intersection->point);
	negate_vec(&intersection->eye, &ray->direction);
	intersection->eye.w = 0;
	intersection->inside = false;
	if (dot_product(&intersection->normal, &intersection->eye) < 0)
	{
		intersection->inside = true;
		negate_vec(&intersection->normal, &intersection->normal);
	}
	scale_vec(&intersection->over_point, &intersection->normal, EPSILON);
	add_vec(&intersection->over_point, &intersection->point,
		&intersection->over_point);
	reflect_vector(&intersection->reflect_vec, &ray->direction,
		&intersection->normal);
}

t_intersection	*hit(t_intersections *xs)
{
	float	min_time;
	int		i;
	int		idx;

	i = 0;
	idx = 0;
	min_time = INFINITY;
	while (i < xs->count)
	{
		if (xs->arr[i].time >= 0 && xs->arr[i].time < min_time)
		{
			min_time = xs->arr[i].time;
			idx = i;
		}
		i++;
	}
	if (min_time == INFINITY)
		return (NULL);
	return (&xs->arr[idx]);
}

void	ray_from_cam(t_ray *ray, const t_camera *cam, float x, float y)
{
	float		world_x;
	float		world_y;
	t_vector	pixel;
	t_vector	world_point;
	t_vector	center;

	world_x = cam->half_width - (x) * cam->pixel_size;
	world_y = cam->half_height - (y) * cam->pixel_size;
	world_point.x = world_x;
	world_point.y = world_y;
	world_point.z = -1;
	world_point.w = 1;
	mat_vec_multiply(&pixel, &cam->inv_trans, &world_point);
	ft_bzero(&center, sizeof(t_vector));
	center.w = 1;
	mat_vec_multiply(&ray->origin, &cam->inv_trans, &center);
	sub_vec(&ray->direction, &pixel, &ray->origin);
	ray->direction.w = 0;
	normalize_vec(&ray->direction);
}

// void	ray_from_cam(t_ray *ray, const t_camera *cam, int x, int y)
// {
// 	float		world_x;
// 	float		world_y;
// 	t_vector	pixel;
// 	t_vector	world_point;
// 	t_vector	center;

// 	world_x = cam->half_width - (x + 0.5) * cam->pixel_size;
// 	world_y = cam->half_height - (y + 0.5) * cam->pixel_size;
// 	world_point.x = world_x;
// 	world_point.y = world_y;
// 	world_point.z = -1;
// 	world_point.w = 1;
// 	mat_vec_multiply(&pixel, &cam->inv_trans, &world_point);
// 	ft_bzero(&center, sizeof(t_vector));
// 	center.w = 1;
// 	mat_vec_multiply(&ray->origin, &cam->inv_trans, &center);
// 	sub_vec(&ray->direction, &pixel, &ray->origin);
// 	ray->direction.w = 0;
// 	normalize_vec(&ray->direction);
// }

void	ray_position(t_vector *pos, const t_ray *ray, float time)
{
	pos->x = ray->direction.x * time + ray->origin.x;
	pos->y = ray->direction.y * time + ray->origin.y;
	pos->z = ray->direction.z * time + ray->origin.z;
	pos->w = 1;
}

void	transform_ray(t_ray *transformed_ray, const t_ray *ray,
	const t_shape *shape)
{
	mat_vec_multiply(&transformed_ray->origin, &shape->inv_transf,
		&ray->origin);
	mat_vec_multiply(&transformed_ray->direction, &shape->inv_transf,
		&ray->direction);
}
