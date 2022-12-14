/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_hook.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 18:50:31 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/05 17:38:10 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mathRT.h"
#include "miniRT.h"

t_vector	*sphere_to_xyz(t_vector *vec, float phi, float theta, float r)
{
	vec->x = r * sin(phi) * cos(theta);
	vec->z = r * sin(phi) * sin(theta);
	vec->y = r * cos(phi);
	vec->w = 0;
	return (vec);
}

void	move_cam(t_scene *scene)
{
	t_vector	vec;

	ft_bzero(&vec, sizeof(t_vector));
	if (scene->keys_held.w == true)
	{
		sphere_to_xyz(&vec, scene->cam.phi, scene->cam.theta, CAM_SPEED);
		add_vec(&scene->cam.position, &scene->cam.position, &vec);
	}
	if (scene->keys_held.a == true)
	{
		sphere_to_xyz(&vec, M_PI_2, scene->cam.theta - M_PI_2, -CAM_SPEED);
		add_vec(&scene->cam.position, &scene->cam.position, &vec);
	}
	if (scene->keys_held.s == true)
	{
		sphere_to_xyz(&vec, scene->cam.phi, scene->cam.theta, -CAM_SPEED);
		add_vec(&scene->cam.position, &scene->cam.position, &vec);
	}
	if (scene->keys_held.d == true)
	{
		sphere_to_xyz(&vec, M_PI_2, scene->cam.theta - M_PI_2, CAM_SPEED);
		add_vec(&scene->cam.position, &scene->cam.position, &vec);
	}
	if (scene->keys_held.q == true)
	{
		vec.y = 0.35;
		add_vec(&scene->cam.position, &scene->cam.position, &vec);
	}
	if (scene->keys_held.e == true)
	{
		vec.y = -0.35;
		add_vec(&scene->cam.position, &scene->cam.position, &vec);
	}
}

void	camera_controls(t_scene *scene)
{
	if (scene->keys_held.w || scene->keys_held.a || scene->keys_held.s
		|| scene->keys_held.d || scene->keys_held.q || scene->keys_held.e)
		move_cam(scene);
	if (scene->keys_held.up == true && scene->cam.phi > 0.2)
		scene->cam.phi -= 0.05;
	if (scene->keys_held.down == true && scene->cam.phi < M_PI - 0.2)
		scene->cam.phi += 0.05;
	if (scene->keys_held.left == true)
		scene->cam.theta += 0.10;
	if (scene->keys_held.right == true)
		scene->cam.theta -= 0.10;
	if (scene->keys_held.up || scene->keys_held.left || scene->keys_held.right
		|| scene->keys_held.down)
		sphere_to_xyz(&scene->cam.dir, scene->cam.phi,
			scene->cam.theta, 1);
}

void	move_object_fwd(t_scene *scene, t_shape *shape)
{
	t_vector	offset;

	ft_bzero(&offset, sizeof(t_vector));
	if (scene->keys_held.w)
	{
		sphere_to_xyz(&offset, M_PI / 2, scene->cam.theta, 0.2);
	}
	if (scene->keys_held.s)
	{
		sphere_to_xyz(&offset, M_PI / 2, scene->cam.theta, -0.2);
	}
	add_vec(&shape->origin, &shape->origin, &offset);
}

void	move_object_h(t_scene *scene, t_shape *shape)
{
	t_vector	offset;
	t_vector	increment;

	ft_bzero(&offset, sizeof(t_vector));
	ft_bzero(&increment, sizeof(t_vector));
	if (scene->keys_held.a)
	{
		sphere_to_xyz(&offset, M_PI_2, scene->cam.theta + M_PI_2, 0.2);
		sphere_to_xyz(&increment, M_PI_2, scene->cam.theta + M_PI_2,
			-0.0001);
	}
	if (scene->keys_held.d)
	{
		sphere_to_xyz(&offset, M_PI_2, scene->cam.theta - M_PI_2, 0.2);
		sphere_to_xyz(&increment, M_PI_2, scene->cam.theta - M_PI_2,
			-0.0001);
	}
	add_vec(&shape->origin, &shape->origin, &offset);
}

void	move_object_v(t_scene *scene, t_shape *shape)
{
	t_vector	offset;
	t_vector	increment;

	ft_bzero(&offset, sizeof(t_vector));
	ft_bzero(&increment, sizeof(t_vector));
	if (scene->keys_held.q)
	{
		offset.y = 0.2;
		increment.y = -0.0001;
	}
	if (scene->keys_held.e)
	{
		offset.y = -0.2;
		increment.y = 0.0001;
	}
	add_vec(&shape->origin, &shape->origin, &offset);
}

void	scale_object(t_scene *scene, t_shape *shape)
{
	if (scene->keys_held.plus == true)
	{
		if (shape->type == CUBE)
		{
			shape->props.scale.x += 0.04;
			shape->props.scale.y += 0.04;
			shape->props.scale.z += 0.04;
		}
		else
		{
			shape->props.radius += 0.04;
			shape->props.scale.x = shape->props.radius;
			if (shape->type == CYLINDER || shape->type == CONE)
			{
				shape->props.scale.y = 1;
			}
			else
				shape->props.scale.y = shape->props.radius;
			shape->props.scale.z = shape->props.radius;
		}
	}
	if (scene->keys_held.minus == true)
	{
		if (shape->type == CUBE)
		{
			if (shape->props.scale.x > 0.3)
				shape->props.scale.x -= 0.04;
			if (shape->props.scale.y > 0.3)
				shape->props.scale.y -= 0.04;
			if (shape->props.scale.z > 0.3)
				shape->props.scale.z -= 0.04;
		}
		else
		{
			if (shape->props.radius > 0.3)
				shape->props.radius -= 0.04;
			shape->props.scale.x = shape->props.radius;
			if (shape->type == CYLINDER || shape->type == CONE)
			{
				shape->props.scale.y = 1;
			}
			else
				shape->props.scale.y = shape->props.radius;
			shape->props.scale.z = shape->props.radius;
		}
	}
	shape->props.radius_squared = shape->props.radius * shape->props.radius;
}

void	change_height(t_scene *scene, t_shape *shape)
{
	if (scene->keys_held.plus == true)
	{
		shape->props.height += 0.04;
	}
	if (scene->keys_held.minus == true)
	{
		if (shape->props.height > 0.2)
			shape->props.height -= 0.04;
	}
}

void	rotate_object_x(t_scene *scene, t_shape *shape, float deg)
{
	t_mat4		rot;
	t_vector	ax;
	t_vector	up;
	t_mat4		mat_copy;

	up.x = 0;
	up.y = 1;
	up.z = 0;
	up.w = 0;
	cross_product(&ax, &up, &scene->cam.dir);
	normalize_vec(&ax);
	if (scene->keys_held.down == true)
		axis_angle(&rot, &ax, -deg);
	else
		axis_angle(&rot, &ax, deg);
	ft_memcpy(&mat_copy, &shape->added_rots, sizeof(t_mat4));
	mat_multiply(&shape->added_rots, &rot, &mat_copy);
}

void	rotate_object_y(t_scene *scene, t_shape *shape, float deg)
{
	t_mat4	rot;
	t_mat4	mat_copy;

	if (scene->keys_held.left == true)
		rotation_matrix_y(&rot, deg);
	else
		rotation_matrix_y(&rot, -deg);
	ft_memcpy(&mat_copy, &shape->added_rots, sizeof(t_mat4));
	mat_multiply(&shape->added_rots, &rot, &mat_copy);
}

void	rotate_object_z(t_scene *scene, t_shape *shape, float deg)
{
	t_mat4	rot;
	t_mat4	mat_copy;

	if (scene->keys_held.left == true)
		axis_angle(&rot, &scene->cam.dir, deg);
	else
		axis_angle(&rot, &scene->cam.dir, -deg);
	ft_memcpy(&mat_copy, &shape->added_rots, sizeof(t_mat4));
	mat_multiply(&shape->added_rots, &rot, &mat_copy);
}

void	transform_object(t_scene *scene)
{
	if (scene->keys_held.w == true || scene->keys_held.s == true)
		move_object_fwd(scene, &scene->shapes[scene->shape_idx]);
	if (scene->keys_held.a == true || scene->keys_held.d == true)
		move_object_h(scene, &scene->shapes[scene->shape_idx]);
	if (scene->keys_held.q == true || scene->keys_held.e == true)
		move_object_v(scene, &scene->shapes[scene->shape_idx]);
	if (scene->keys_held.shift == false
		&& (scene->keys_held.plus == true || scene->keys_held.minus == true))
		scale_object(scene, &scene->shapes[scene->shape_idx]);
	if (scene->keys_held.shift == true
		&& (scene->keys_held.plus == true || scene->keys_held.minus == true))
		change_height(scene, &scene->shapes[scene->shape_idx]);
	if (scene->keys_held.shift == false
		&& (scene->keys_held.left == true || scene->keys_held.right == true))
		rotate_object_y(scene, &scene->shapes[scene->shape_idx], DEG_TO_RAD * 5);
	if (scene->keys_held.shift == true
		&& (scene->keys_held.left == true || scene->keys_held.right == true))
		rotate_object_z(scene, &scene->shapes[scene->shape_idx], DEG_TO_RAD * 5);
	if (scene->keys_held.up == true || scene->keys_held.down == true)
		rotate_object_x(scene, &scene->shapes[scene->shape_idx], DEG_TO_RAD * 5);
	if (scene->settings.collisions == true && (scene->keys_held.w
			|| scene->keys_held.a || scene->keys_held.s || scene->keys_held.d
			|| scene->keys_held.up || scene->keys_held.right
			|| scene->keys_held.q || scene->keys_held.e || scene->keys_held.down
			|| scene->keys_held.left || scene->keys_held.plus
			|| scene->keys_held.minus))
		collide(scene, true, 100, &scene->shapes[scene->shape_idx]);
	if (scene->keys_held.plus == true)
	{
		if (collide(scene, false, 1, NULL) == true)
		{
			scene->keys_held.plus = false;
			scene->keys_held.minus = true;
			if (scene->keys_held.shift == false)
				scale_object(scene, &scene->shapes[scene->shape_idx]);
			else
				change_height(scene, &scene->shapes[scene->shape_idx]);
			scene->keys_held.minus = false;
			calculate_transforms(scene);
			draw_scene(scene);
		}
	}
}

void	light_controls(t_scene *scene)
{
	if (scene->keys_held.up == true)
		scene->lights[0].position.y += 0.3;
	if (scene->keys_held.down == true)
		scene->lights[0].position.y -= 0.3;
	if (scene->keys_held.left == true)
		scene->lights[0].position.x -= 0.3;
	if (scene->keys_held.right == true)
		scene->lights[0].position.x += 0.3;
}

void	reset_look_at(t_scene *scene)
{
	if (scene->cam.dir.x > 0)
		scene->cam.theta = atan(scene->cam.dir.z / scene->cam.dir.x);
	else if (scene->cam.dir.x < 0 && scene->cam.dir.z >= 0)
		scene->cam.theta = atan(scene->cam.dir.z / scene->cam.dir.x) \
		+ M_PI;
	else if (scene->cam.dir.x < 0 && scene->cam.dir.z < 0)
		scene->cam.theta = atan(scene->cam.dir.z / scene->cam.dir.x) \
		- M_PI;
	scene->cam.phi = acos(scene->cam.dir.y);
	scene->look_at.trigger = false;
	scene->look_at.step_num = 0;
}

void	look_at_animation(t_scene *scene)
{
	t_vector	pos_step;
	t_vector	dir_step;

	scale_vec(&pos_step, &scene->look_at.pos_diff, \
		1.0 / scene->look_at.step_amount);
	scale_vec(&dir_step, &scene->look_at.dir_diff, \
		1.0 / scene->look_at.step_amount);
	add_vec(&scene->cam.position, &scene->cam.position, &pos_step);
	add_vec(&scene->look_at.current_dir, &scene->look_at.current_dir,
		&dir_step);
	scene->cam.dir = scene->look_at.current_dir;
	calculate_transforms(scene);
	draw_scene(scene);
	scene->look_at.step_num++;
	if (scene->look_at.step_num >= scene->look_at.step_amount)
		reset_look_at(scene);
}

int	render_loop(t_scene *scene)
{
	if (scene->settings.camera_mode == true
		&& scene->settings.edit_mode == true)
	{
		camera_controls(scene);
	}
	else if (scene->settings.edit_mode == true)
	{
		transform_object(scene);
		mouse_rotate(scene);
	}
	if (scene->look_at.trigger == true && scene->settings.edit_mode == true)
		look_at_animation(scene);
	if (scene->look_at.trigger == false && scene->settings.edit_mode == true
			&& (scene->keys_held.w || scene->keys_held.a || scene->keys_held.s
			|| scene->keys_held.d|| scene->keys_held.up
			|| scene->keys_held.right || scene->keys_held.q
			|| scene->keys_held.e || scene->keys_held.down
			|| scene->keys_held.left || scene->keys_held.plus
			|| scene->keys_held.minus))
	{
		calculate_transforms(scene);
		draw_scene(scene);
	}
	return (0);
}
