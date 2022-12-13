/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 14:33:36 by mkhan             #+#    #+#             */
/*   Updated: 2022/12/10 11:27:25 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	view_transform(t_mat4 *res, const t_vector *from, const t_vector *up,
	const t_vector *forward)
{
	t_vector	left;
	t_vector	true_up;
	t_vector	upn;
	t_mat4		orientation;
	t_mat4		translation;

	ft_memcpy(&upn, up, sizeof(t_vector));
	normalize_vec(&upn);
	cross_product(&left, forward, &upn);
	cross_product(&true_up, &left, forward);
	ft_bzero(&orientation, sizeof(t_mat4));
	orientation[0][0] = left.x;
	orientation[0][1] = left.y;
	orientation[0][2] = left.z;
	orientation[1][0] = true_up.x;
	orientation[1][1] = true_up.y;
	orientation[1][2] = true_up.z;
	orientation[2][0] = forward->x * -1;
	orientation[2][1] = forward->y * -1;
	orientation[2][2] = forward->z * -1;
	orientation[3][3] = 1;
	translate_matrix(&translation, from->x * -1, from->y * -1, from->z * -1);
	mat_multiply(res, &orientation, &translation);
}

void	camera_init(t_camera *camera, t_scene *scene)
{
	double	half_view;
	double	aspect;
	double	h;
	double	w;

	h = scene->render_h;
	w = scene->render_w;
	if (scene->edit_mode == true)
	{
		h = scene->edit_h;
		w = scene->edit_w;
	}
	half_view = tan((camera->fov / 2.0f) * M_PI / 180.0f);
	aspect = w / h;
	if (aspect >= 1)
	{
		camera->half_width = half_view;
		camera->half_height = half_view / aspect;
	}
	else
	{
		camera->half_width = half_view * aspect;
		camera->half_height = half_view;
	}
	camera->pixel_size = (camera->half_width * 2) / w;
}

void	ray_for_pixel(t_ray *ray, const t_camera *cam, int x, int y)
{
	double		world_x;
	double		world_y;
	t_vector	pixel;
	t_vector	world_point;
	t_vector	center;

	world_x = cam->half_width - (x + 0.5) * cam->pixel_size;
	world_y = cam->half_height - (y + 0.5) * cam->pixel_size;
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
