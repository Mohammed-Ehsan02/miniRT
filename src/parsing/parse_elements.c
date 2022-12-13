/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/20 16:32:52 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/10 12:53:41 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/**
 * @brief Parses a point light according to the specification provided in the
 *  scene file
 * @param scene The scene struct to fill with the light configuration
 * @param splitted The tokenized line containing the configuration of the light
 * @param line The line where the light is located
 * @param line_num The line number where the light is located
 * @return True if the light was parsed correctly
 */
bool	parse_light(t_scene *scene, char **splitted, char *line,
	size_t line_num)
{
	t_light	*light;
	bool	success;

	success = true;
	if (scene->count.light_count == LIGHT_MAX || split_count(splitted) != 4)
		return (light_parse_error(line, line_num, scene));
	if (scene->lights == NULL)
		scene->lights = ft_calloc(LIGHT_MAX, sizeof(t_light));
	if (scene->lights == NULL)
		return (light_parse_error(line, line_num, scene));
	light = &scene->lights[scene->count.light_count];
	parse_coordinates(&light->position, splitted[1], &success);
	light->position.w = 1;
	if (success == false)
		return (light_parse_error(line, line_num, scene));
	light->intensity = ft_atof(splitted[2], &success);
	if (success == false || light->intensity < 0.0 || light->intensity > 1.0)
		return (light_parse_error(line, line_num, scene));
	parse_color(&light->color, splitted[3], &success);
	if (success == false)
		return (light_parse_error(line, line_num, scene));
	scene->count.light_count++;
	return (true);
}

/**
 * @brief Parses an ambient light according to the specification provided in the
 *  scene file
 * @param scene The scene struct to fill with the ambient light configuration
 * @param splitted The tokenized line containing the configuration of the 
 * ambient light
 * @param line_num The line number where the ambient light is located
 * @param line The line where the ambient light is located
 * @return True if the ambient light was parsed correctly
 */
bool	parse_ambient(t_scene *scene, char **splitted, size_t line_num,
		char *line)
{
	bool	success;

	success = true;
	if (split_count(splitted) != 3)
	{
		ambient_parse_error(line, line_num, scene);
		return (false);
	}
	scene->ambient.intensity = ft_atof(splitted[1], &success);
	if (success == false || scene->ambient.intensity < 0.0
		|| scene->ambient.intensity > 1.0)
	{
		ambient_parse_error(line, line_num, scene);
		return (false);
	}
	parse_color(&scene->ambient.color, splitted[2], &success);
	if (success == false)
	{
		ambient_parse_error(line, line_num, scene);
		return (false);
	}
	scene->count.ambient_count++;
	return (true);
}

/**
 * @brief Parses a camera according to the specification provided in the
 *  scene file
 * @param scene The scene struct to fill with the camera information
 * @param splitted The tokenized line containing the camera configuration
 * @param line The line containing the camera configuration
 * @param line_num The line number containing the camera
 * @return True if the camera was parsed correctly
 */
bool	parse_camera(t_scene *scene, char **splitted, char *line,
		size_t line_num)
{
	bool	success;

	success = true;
	if (split_count(splitted) != 4)
		return (camera_parse_error(line, line_num, scene, false));
	parse_coordinates(&scene->camera.position, splitted[1], &success);
	scene->camera.position.w = 1;
	if (success == false)
		return (camera_parse_error(line, line_num, scene, true));
	parse_orientation(&scene->camera.orientation, splitted[2], &success);
	if (success == false || vec_magnitude(&scene->camera.orientation) == 0 ||
		(scene->camera.orientation.x == 0 && scene->camera.orientation.y == 1 &&
		scene->camera.orientation.z == 0))
		return (camera_parse_error(line, line_num, scene, false));
	scene->camera.fov = ft_atol(splitted[3], &success);
	if (is_num(splitted[3], false) == false || success == false || scene->camera.fov < 0 || scene->camera.fov > 180)
		return (camera_parse_error(line, line_num, scene, false));
	scene->count.camera_count++;
	return (true);
}
