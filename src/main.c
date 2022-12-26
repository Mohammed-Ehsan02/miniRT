/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 14:01:06 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/26 19:13:33 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/**
 * @brief Opens a file and checks for errors with the arguments provided to
 * the executable
 * @param argc The number of arguments provided to the executable
 * @param argv The arguments provided to the executable
 * @return The file descriptor of the file passed to the executable or -1 on 
 * error
 */
static int	open_file(int argc, char **argv)
{
	char	*file_name;
	int		fd;

	if (argc != 2)
	{
		printf("Please give me a .rt scene file\n");
		return (-1);
	}
	file_name = argv[1];
	if (ft_strnstr(file_name, ".rt", ft_strlen(file_name)) == NULL
		|| ft_strncmp(&file_name[ft_strlen(file_name) - 3], ".rt", 3) != 0)
	{
		printf(RED"Can only read .rt files\n"RESET);
		return (-1);
	}
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
	{
		printf(RED"Could not open file \"%s\"\n"RESET, file_name);
		return (-1);
	}
	return (fd);
}

void	init_display(t_display *disp, t_settings *settings)
{
	disp->mlx = mlx_init();
	disp->win = mlx_new_window(disp->mlx, settings->display_w,
			settings->display_h, "MiniRT");
	disp->render_img = mlx_new_image(disp->mlx, settings->render_w,
			settings->render_h);
	disp->edit_img = mlx_new_image(disp->mlx, 1920 * 3, 1080 * 3);
	disp->display_img = mlx_new_image(disp->mlx, settings->display_w,
			settings->display_h);
	disp->render_addr = mlx_get_data_addr(disp->render_img,
			&disp->bytes_per_pixel, &disp->line_length, &disp->endian);
	disp->display_addr = mlx_get_data_addr(disp->display_img,
			&disp->bytes_per_pixel, &disp->line_length, &disp->endian);
	disp->edit_addr = mlx_get_data_addr(disp->edit_img, &disp->bytes_per_pixel,
			&disp->line_length, &disp->endian);
	disp->info_img = mlx_new_image(disp->mlx, settings->display_w * 0.16,
			settings->display_h);
	disp->info_addr = mlx_get_data_addr(disp->info_img, &disp->bytes_per_pixel,
			&disp->line_length, &disp->endian);
	disp->bytes_per_pixel /= 8;
}

void	init_settings(t_settings *settings)
{
	settings->render_scale = 1;
	settings->edit_scale = 0.8;
	settings->render_w = 1920 * settings->render_scale;
	settings->render_h = 1080 * settings->render_scale;
	settings->edit_w = 1920 * settings->edit_scale;
	settings->edit_h = 1080 * settings->edit_scale;
	settings->display_w = 1920 * 0.6;
	settings->display_h = 1080 * 0.6;
	settings->collisions = true;
	settings->reflection_depth = 1;
}

// To enable transparency
// scene->shapes[0].props.transparency = 1;
// scene->shapes[0].props.reflectiveness = 0.9;
// scene->shapes[0].props.ior = 1.5;
// scene->shapes[0].props.diffuse = 0;
// ! Put this somewhere
// ! free_scene(scene);
int	main(int argc, char **argv)
{
	t_scene		*scene;
	t_display	disp;
	int			fd;

	fd = open_file(argc, argv);
	if (fd == -1)
		return (EXIT_FAILURE);
	scene = parse_scene(fd);
	close(fd);
	if (scene == NULL)
		return (EXIT_FAILURE);
	init_settings(&scene->settings);
	scene->shapes[0].props.highlighted = true;
	sem_unlink("/loading");
	scene->sem_loading = sem_open("/loading", O_CREAT, 0644, 0);
	init_display(&disp, &scene->settings);
	scene->disp = &disp;
	mlx_hook(disp.win, 2, (1L << 0), key_press, scene);
	mlx_hook(disp.win, 3, (1L << 1), key_release, scene);
	mlx_hook(disp.win, 4, (1L<<2), mouse_down, scene);
	mlx_hook(disp.win, 5, (1L<<3), mouse_up, scene);
	mlx_loop_hook(disp.mlx, render_loop, scene);
	camera_init(&scene->camera, scene);
	scene->camera.theta = atan(scene->camera.dir.z / scene->camera.dir.x);
	scene->camera.phi = acos(scene->camera.dir.y);
	calculate_transforms(scene);
	draw_scene(scene);
	mlx_loop(disp.mlx);
	return (EXIT_SUCCESS);
}
