/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 15:43:11 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/10 12:54:58 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/**
 * @brief Prints the appropriate error message when an error occurs while
 * parsing a point light
 * @param line Line where the parse error occured
 * @param line_num Number of the line where the error occured
 * @param scene Pointer to the scene struct
 * @param splitted Array to be freed
 * @param fd fd to be closed
 */
void	*light_parse_error(char *line, size_t line_num, t_scene *scene)
{
	t_light	*light;

	if (scene->lights == NULL)
		light = NULL;
	else
		light = &scene->lights[scene->count.light_count];
	if (light != NULL && scene->count.light_count >= LIGHT_MAX)
		printf(RED"Error: Scene contains more than %d lights\n"
			RESET, LIGHT_MAX);
	else if (light && (light->intensity < 0.0 || light->intensity > 1.0))
	{
		printf(YELLOW"Error with parsing light intensity on line #%ld\n"
			RED"->\t%s\n"RESET, line_num, line);
		printf(YELLOW"The intensity value is out of range\n"RESET);
	}
	else if (!light || !check_color(&light->color, line_num, line, "light"))
		printf(YELLOW"Error with parsing light on line #%ld\n"RED"->\t%s"RESET
			YELLOW"Correct syntax is \"L [origin] [intensity] [color\"\n"RESET,
			line_num, line);
	free_scene(scene);
	get_next_line(-1);
	return (NULL);
}

/**
 * @brief Prints an error message if an identifier is unrecognized
 * @param line Line where the identifier was located
 * @param line_num Number of the line where the identifier was located
 * @param scene Scene struct to be freed
 * @param splitted Array to be freed
 * @param fd File descriptor to be closed
 */
void	*unknown_identifier(char *line, size_t line_num, t_scene *scene,
		char **splitted)
{
	printf(YELLOW"Unknown identifier \"%s\" on line #%ld\n"RED"->\t%s"RESET,
		splitted[0], line_num, line);
	free_scene(scene);
	get_next_line(-1);
	return (NULL);
}

/**
 * @brief Prints the appropriate error message when an error occurs while
 * parsing an ambient light
 * @param line Line where the parse error occured
 * @param line_num Number of the line where the error occured
 * @param scene Pointer to the scene struct
 * @param splitted Array to be freed
 * @param fd fd to be closed
 */
void	*ambient_parse_error(char *line, size_t line_num, t_scene *scene)
{
	bool	color;

	color = check_color(&scene->ambient.color, line_num, line, "ambient light");
	if (!color && (scene->ambient.intensity < 0.0
			|| scene->ambient.intensity > 1.0))
	{
		printf(YELLOW"Ambient light intensity out of range on line #%ld\n"
			RED"->\t%s"RESET"Range is [0.0 -> 1.0]\n", line_num, line);
	}
	else if (!color)
	{
		printf(YELLOW"Error with parsing ambient light on line #%ld\n"
			RED"->\t%s"YELLOW"Correct syntax is \"A [intensity] [color]\"\n"
			RESET, line_num, line);
	}
	free_scene(scene);
	get_next_line(-1);
	return (NULL);
}

/**
 * @brief Prints the appropriate error message when an error occurs while
 * parsing a camera
 * @param line Line where the parse error occured
 * @param line_num Number of the line where the error occured
 * @param scene Pointer to the scene struct
 * @param splitted Array to be freed
 * @param fd fd to be closed
 */
void	*camera_parse_error(char *line, size_t line_num, t_scene *scene,
	bool invalid_coords)
{
	bool	orientation;

	orientation = false;
	if (!invalid_coords)
		orientation = check_orientation(&scene->camera.orientation, line_num, line,
			"camera");
	if (!invalid_coords && vec_magnitude(&scene->camera.orientation) == 1 && scene->camera.orientation.y == 1)
	{
		printf(YELLOW"Error with parsing camera orientation on line #%ld\n"
			RED"->\t%s\n"RESET, line_num, line);
		printf(YELLOW"Camera orientation cannot be the up vector (0, 1, 0)\n"
			RESET);
	}
	else if (!invalid_coords && !orientation && (scene->camera.fov < 0 || scene->camera.fov > 180))
	{
		printf(YELLOW"Error with parsing camera fov on line #%ld\n"RED"->\t%s\n"
			RESET, line_num, line);
		printf(YELLOW"The fov value is out of range\n"RESET);
	}
	else if (invalid_coords || !orientation)
		printf(YELLOW"Error with parsing camera on line #%ld\n"RED"->\t%s"RESET
			YELLOW"Correct syntax is \"C [origin] [orientation] [fov]\"\n"RESET,
			line_num, line);
	free_scene(scene);
	get_next_line(-1);
	return (NULL);
}
