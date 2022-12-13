/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_shapes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/20 16:29:40 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/07 00:01:52 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/**
 * @brief Returns whether an identifier is shape
 * @param identifier 
 * @return True if the identifier is a shape
 */
bool	is_shape(const char *identifier)
{
	return (ft_strcmp(identifier, "sp") == 0
		|| ft_strcmp(identifier, "pl") == 0
		|| ft_strcmp(identifier, "cy") == 0
		|| ft_strcmp(identifier, "cu") == 0);
}

/**
 * @brief Parses a sphere object
 * @param shape The shape to parse
 * @param splitted The tokens containing the sphere configuration
 * @param success A boolean pointer set to false if there are any errors while
 *  parsing
 */
static void	parse_sphere(t_shape *shape, char **splitted, bool *success)
{
	bool	parse_success;

	parse_success = true;
	shape->type = SPHERE;
	if (split_count(splitted) != 4)
	{
		*success = false;
		shape->radius = 0.1;
		return ;
	}
	shape->radius = ft_atof(splitted[2], success) / 2;
	if (*success == false || shape->radius <= 0.0)
		parse_success = false;
	parse_coordinates(&shape->origin, splitted[1], success);
	shape->origin.w = 1;
	if (*success == false)
		parse_success = false;
	parse_color(&shape->color, splitted[3], success);
	if (*success == false)
		parse_success = false;
	*success = parse_success;
	shape->reflectiveness = 0.1;
}

/**
 * @brief Parses a plane object
 * @param shape The shape to parse
 * @param splitted The tokens containing the plane configuration
 * @param success A boolean pointer set to false if there are any errors while
 *  parsing
 */
static void	parse_plane(t_shape *shape, char **splitted, bool *success)
{
	bool	parse_success;

	parse_success = true;
	shape->type = PLANE;
	if (split_count(splitted) != 4)
	{
		*success = false;
		return ;
	}
	parse_coordinates(&shape->origin, splitted[1], success);
	shape->origin.w = 1;
	if (*success == false)
		parse_success = false;
	parse_orientation(&shape->orientation, splitted[2], success);
	if (*success == false || vec_magnitude(&shape->orientation) == 0)
		parse_success = false;
	parse_color(&shape->color, splitted[3], success);
	if (*success == false)
		parse_success = false;
	*success = parse_success;
	shape->reflectiveness = 0.05;
}

/**
 * @brief Parses a cylinder object
 * @param shape The shape to parse
 * @param splitted The tokens containing the cylinder configuration
 * @param success A boolean pointer set to false if there are any errors while
 *  parsing
 */
static void	parse_cylinder(t_shape *shape, char **splitted, bool *success)
{
	bool	parse_success;

	parse_success = true;
	shape->type = CYLINDER;
	if (split_count(splitted) != 6)
		*success = false;
	if (*success == false)
		return ;
	shape->radius = ft_atof(splitted[3], success) / 2;
	if (*success == false || shape->radius <= 0.0)
		parse_success = false;
	shape->height = ft_atof(splitted[4], success);
	if (*success == false || shape->height <= 0.0)
		parse_success = false;
	parse_coordinates(&shape->origin, splitted[1], success);
	shape->origin.w = 1;
	if (*success == false)
		parse_success = false;
	parse_orientation(&shape->orientation, splitted[2], success);
	if (*success == false || vec_magnitude(&shape->orientation) == 0)
		parse_success = false;
	parse_color(&shape->color, splitted[5], success);
	if (*success == false)
		parse_success = false;
	*success = parse_success;
}

/**
 * @brief Parses a shape
 * @param scene Pointer to scene struct
 * @param splitted The line containing the shape configurations
 * @param line_num The number of the line containing the shape
 * @param line The line containing the shape
 * @return Whether the parsing succeeded or not
 */
bool	parse_shape(t_scene *scene, char **splitted, size_t line_num,
		char *line)
{
	t_shape	*shape;
	bool	success;

	success = true;
	if (scene->count.shape_count == SHAPE_MAX)
		return (shape_parse_error(line, line_num, scene, splitted));
	if (scene->shapes == NULL)
		scene->shapes = ft_calloc(SHAPE_MAX, sizeof(t_shape));
	if (scene->shapes == NULL)
		return (shape_parse_error(line, line_num, scene, splitted));
	shape = &scene->shapes[scene->count.shape_count];
	if (ft_strcmp(splitted[0], "sp") == 0)
		parse_sphere(shape, splitted, &success);
	else if (ft_strcmp(splitted[0], "pl") == 0)
		parse_plane(shape, splitted, &success);
	else if (ft_strcmp(splitted[0], "cy") == 0)
		parse_cylinder(shape, splitted, &success);
	if (success == false)
		return (shape_parse_error(line, line_num, scene, splitted));
	scene->count.shape_count++;
	shape->diffuse = 0.9;
	shape->specular = 0.9;
	shape->shininess = 200;
	return (true);
}
