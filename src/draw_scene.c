/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 20:19:41 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/12 19:27:56 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	prepare_computations(t_intersect *intersection, t_ray *ray)
{
	ray_position(&intersection->point, ray, intersection->time);
	intersection->normal = normal_at(intersection->shape, &intersection->point);
	negate_vec(&intersection->eye, &ray->direction);
	intersection->eye.w = 0;
	if (dot_product(&intersection->normal, &intersection->eye) < 0)
	{
		intersection->inside = true;
		negate_vec(&intersection->normal, &intersection->normal);
	}
	else
		intersection->inside = false;
	scale_vec(&intersection->over_point, &intersection->normal, 0.001);
	add_vec(&intersection->over_point, &intersection->point,
		&intersection->over_point);
	reflect(&intersection->reflect_vec, &ray->direction, &intersection->normal);
}

int	min(int a, int b)
{
	if (a < b)
		return (a);
	return (b);
}

void	init_workers(t_worker *workers, t_scene *scene)
{
	unsigned int	i;

	i = 0;
	while (i < NUM_THREADS)
	{
		if (scene->edit_mode == true)
		{
			workers[i].height = scene->edit_h;
			workers[i].width = scene->edit_w;
			workers[i].addr = scene->mlx->edit_addr;
		}
		else
		{
			workers[i].height = scene->render_h;
			workers[i].width = scene->render_w;
			workers[i].addr = scene->mlx->render_addr;
		}
		workers[i].max_workers = NUM_THREADS;
		workers[i].worker_id = i;
		workers[i].scene = scene;
		workers[i].y_start = (workers[i].height / (float)NUM_THREADS) * i;
		workers[i].y_end = (workers[i].height / (float)NUM_THREADS) * (i + 1);
		workers[i].y_scale_start = (scene->display_h / (float)NUM_THREADS) * i;
		workers[i].y_scale_end = (scene->display_h / (float)NUM_THREADS) * (i + 1);
		i++;
	}
}

void	calculate_lighting(t_intersections *arr, t_worker *worker, t_ray *ray,
	int pixel)
{
	t_intersect		*itx;
	unsigned int	light_idx;
	t_color			final_color;
	t_color			light_color;

	itx = hit(arr);
	if (itx != NULL)
	{
		prepare_computations(itx, ray);
		ft_bzero(&final_color, sizeof(t_color));
		light_idx = 0;
		while (light_idx < worker->scene->count.light_count)
		{
			light_color = lighting(itx, worker->scene, light_idx);
			t_color	reflected = reflected_color(worker->scene, itx, worker->scene->reflection_depth, light_idx);
			add_colors(&final_color, &final_color, &light_color);
			add_colors(&final_color, &final_color, &reflected);
			light_idx++;
		}
		itx->shape->mlx_color = create_mlx_color(&final_color);
			*(int *)(worker->addr + pixel) = itx->shape->mlx_color;
		
	}
}

void	*render_scene(t_worker *worker)
{
	t_intersections	arr;
	int				x;
	int				y;
	unsigned int	shape_idx;
	t_ray			ray;

	y = worker->y_start - 1;
	while (++y < worker->y_end)
	{
		x = -1;
		while (++x < worker->width)
		{
			*(unsigned int *)(worker->addr + \
				(y * worker->width + x) * \
				worker->scene->mlx->bytes_per_pixel) = 0;
			ray_for_pixel(&ray, &worker->scene->camera, x, y);
			shape_idx = -1;
			arr.count = 0;
			while (++shape_idx < worker->scene->count.shape_count)
				intersect(&worker->scene->shapes[shape_idx], &ray, &arr);
			calculate_lighting(&arr, worker, &ray, (y * worker->width \
				+ x) * worker->scene->mlx->bytes_per_pixel);
		}
	}
	return (NULL);
}

void	*nearest_neighbours_scaling(t_worker *worker)
{
	int			x;
	int			y;
	int			src_x;
	int			src_y;


	y = worker->y_scale_start - 1;
	while (++y < worker->y_scale_end)
	{
		x = -1;
		while (++x < worker->scene->display_w)
		{
			src_x = round((x / (float)worker->scene->display_w) * \
			worker->width);
			src_y = round((y / (float)worker->scene->display_h) * \
			worker->height);
			src_x = min(src_x, worker->width - 1);
			src_y = min(src_y, worker->height - 1);
			*(unsigned int *)(worker->scene->mlx->display_addr + (y * \
			worker->scene->display_w + x) * worker->scene->mlx->bytes_per_pixel) = \
			*(unsigned int *)(worker->addr + (src_y * \
			worker->width + src_x) * \
			worker->scene->mlx->bytes_per_pixel);
		}
	}
	return (NULL);
}

void	my_mlx_pixel_put(t_scene *scene, int x, int y, int color)
{
	char	*dst;
	
	if (x > 0 && y > 0 && x < scene->edit_w && y < scene->edit_h)
	{

		dst = scene->mlx->edit_addr + (y * scene->edit_w + x) * scene->mlx->bytes_per_pixel;
		*(unsigned int*)dst = color;
	}
}

void	draw_shape_info(t_scene *scene)
{
	if (scene->edit_mode == false)
		return ;
	unsigned int	shape_idx = 0;
	t_shape	*shape;
	t_vector	cam_point;
	t_vector	info_point;

	while (shape_idx < scene->count.shape_count)
	{
		shape = &scene->shapes[shape_idx];
		if (shape->highlighted == false)
		{
			shape_idx++;
			continue;
		}
		ft_memcpy(&info_point, &shape->origin, sizeof(t_vector));
		info_point.y += shape->radius * 1.5;
		mat_vec_multiply(&cam_point, &scene->camera.transform, &info_point);
		if (shape->type == SPHERE)
		{
			cam_point.x /= -cam_point.z;
			cam_point.y /= -cam_point.z;
			cam_point.x = (cam_point.x + scene->camera.half_width) / (scene->camera.half_width * 2);
			cam_point.y = (cam_point.y + scene->camera.half_height) / (scene->camera.half_height * 2);
			cam_point.x = 1 - cam_point.x;
			cam_point.y = 1 - cam_point.y;

			char str[1000];
			mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, (int)(cam_point.x * scene->display_w) + 5, (int)(cam_point.y * scene->display_h) - 2, 0xffffff, "Sphere");
			sprintf(str, "x: %.2f", shape->origin.x);
			mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, (int)(cam_point.x * scene->display_w) + 5, (int)(cam_point.y * scene->display_h) + 15, 0xffffff, str);
			sprintf(str, "y: %.2f", shape->origin.y);
			mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, (int)(cam_point.x * scene->display_w) + 5, (int)(cam_point.y * scene->display_h) + 32, 0xffffff, str);
			sprintf(str, "z: %.2f", shape->origin.z);
			mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, (int)(cam_point.x * scene->display_w) + 5, (int)(cam_point.y * scene->display_h) + 49, 0xffffff, str);
		}
		
		shape_idx++;
	}
}

void	draw_shape_marker(t_scene *scene)
{
	if (scene->edit_mode == false)
		return ;
	unsigned int	shape_idx = 0;
	t_shape	*shape;
	t_vector	cam_point;

	while (shape_idx < scene->count.shape_count)
	{
		shape = &scene->shapes[shape_idx];
		if (shape->highlighted == false)
		{
			shape_idx++;
			continue;
		}
		mat_vec_multiply(&cam_point, &scene->camera.transform, &shape->origin);
		if (shape->type == SPHERE)
		{
			cam_point.x /= -cam_point.z;
			cam_point.y /= -cam_point.z;
			cam_point.x = (cam_point.x + scene->camera.half_width) / (scene->camera.half_width * 2);
			cam_point.y = (cam_point.y + scene->camera.half_height) / (scene->camera.half_height * 2);
			cam_point.x = 1 - cam_point.x;
			cam_point.y = 1 - cam_point.y;
			my_mlx_pixel_put(scene, (int)(cam_point.x * scene->edit_w), (int)(cam_point.y  * scene->edit_h) , 0x00ffff);
		}
		shape_idx++;
	}
}

/**
 * @brief Draws a scene
 * @param scene A struct describing the current scene
 */
void	draw_scene(t_scene *scene)
{
	t_mlx			*mlx;
	t_worker		workers[NUM_THREADS];
	pthread_t		threads[NUM_THREADS];
	struct timespec	start;
	struct timespec	finish;
	double			elapsed;
	int				i;

	mlx = scene->mlx;
	init_workers(workers, scene);
	clock_gettime(CLOCK_MONOTONIC, &start);
	i = 0;
	while (i < NUM_THREADS)
	{
		pthread_create(&threads[i], NULL, (void *)render_scene, &workers[i]);
		i++;
	}
	i = 0;
	while (i < NUM_THREADS)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	clock_gettime(CLOCK_MONOTONIC, &finish);
	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	printf("render time is %f\n", elapsed);
	clock_gettime(CLOCK_MONOTONIC, &start);
	i = 0;
	draw_shape_marker(scene);
	while (i < NUM_THREADS)
	{
		pthread_create(&threads[i], NULL, (void *)nearest_neighbours_scaling,
			&workers[i]);
		i++;
	}
	i = 0;
	while (i < NUM_THREADS)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	clock_gettime(CLOCK_MONOTONIC, &finish);
	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	printf("scale time is %f\n", elapsed);
	mlx_put_image_to_window(mlx->mlx, mlx->mlx_win, mlx->display_img, 0, 0);
	draw_shape_info(scene);
}
