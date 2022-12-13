/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_attributes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/20 16:31:38 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/10 12:44:22 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/**
 * @brief Checks whether the orientation vector is valid and prints an
 *  appropriate error message is it is not
 * @param orientation The orientation vector to check
 * @param line_num The line number where the vector is located
 * @param line The line where the vector is located
 * @param element The type of element the vector belongs to
 * @return True if the vector is invalid
 */
bool	check_orientation(const t_vector *orientation, size_t line_num,
	const char *line, const char *element)
{
	if (orientation->x < -1.0 || orientation->x > 1.0
		|| orientation->y < -1.0 || orientation->y > 1.0
		|| orientation->z < -1.0 || orientation->z > 1.0
		|| vec_magnitude(orientation) == 0.0)
	{
		printf(YELLOW"Error with parsing %s orientation on line #%ld\n"
			RED"->\t%s\n"RESET, element, line_num, line);
		if (orientation->x < -1.0 || orientation->x > 1.0)
			printf(YELLOW"The x value is out of range\n"RESET);
		if (orientation->y < -1.0 || orientation->y > 1.0)
			printf(YELLOW"The y value is out of range\n"RESET);
		if (orientation->z < -1.0 || orientation->z > 1.0)
			printf(YELLOW"The z value is out of range\n"RESET);
		if (vec_magnitude(orientation) == 0.0)
			printf(YELLOW"The orientation vector cannot be the zero vector"
			"\n"RESET);
		return (true);
	}
	return (false);
}

/**
 * @brief Checks wheter the color is valid and prints an
 *  appropriate error message is it is not
 * @param color The color to check
 * @param line_num The line number where the color is located
 * @param line The line where the color is located
 * @param element The type of element the color belongs to
 * @return True if the color is invalid
 */
bool	check_color(const t_color *color, size_t line_num, const char *line,
	const char *element)
{
	if (color->r < 0 || color->r > 255
		|| color->g < 0 || color->g > 255
		|| color->b < 0 || color->b > 255)
	{
		printf(YELLOW"Error with parsing %s color on line #%ld\n"
			RED"->\t%s\n"RESET, element, line_num, line);
		if (color->r < 0 || color->r > 255)
			printf(YELLOW"The red value is out of range\n"RESET);
		if (color->g < 0 || color->g > 255)
			printf(YELLOW"The green value is out of range\n"RESET);
		if (color->b < 0 || color->b > 255)
			printf(YELLOW"The blue value is out of range\n"RESET);
		return (true);
	}
	return (false);
}

/**
 * @brief Parse the red, green, and blue values of a color
 * @param color Color struct to be filled with RGB values
 * @param str Raw string in the form
 * @param success Boolean pointer that is set to false on error
 */
void	parse_color(t_color *color, const char *str, bool *success)
{
	char	**rgb;
	double	res[3];
	bool	parse_success;
	size_t	i;

	parse_success = true;
	rgb = ft_split(str, ',');
	if (!rgb || count_commas(str) != 2 || split_count(rgb) != 3)
	{
		*success = false;
		free_split_array(rgb);
		return ;
	}
	i = -1;
	while (rgb[++i] != NULL)
	{
		res[i] = ft_atol(rgb[i], success) / 255.0f;
		if (!is_num(rgb[i], false) || res[i] < 0 || res[i] > 1.0f || !*success)
			parse_success = false;
	}
	free_split_array(rgb);
	color->r = res[0];
	color->g = res[1];
	color->b = res[2];
	*success = parse_success;
}

/**
 * @brief Parses given coordinates into a vector
 * @param position Pointer to a vector that will be filled with coordinates
 * @param str Raw string to be parsed
 * @param success Boolean pointer that will be set to false on error
 */
void	parse_coordinates(t_vector *position, const char *str, bool *success)
{
	double	res[3];
	char	**coords;
	bool	parse_success;
	size_t	i;

	parse_success = true;
	coords = ft_split(str, ',');
	if (!coords || count_commas(str) != 2 || split_count(coords) != 3)
	{
		*success = false;
		free_split_array(coords);
		return ;
	}
	i = -1;
	while (coords[++i] != NULL)
	{
		res[i] = ft_atof(coords[i], success);
		if (!is_num(coords[i], true) || *success == false)
			parse_success = false;
	}
	free_split_array(coords);
	position->x = res[0];
	position->y = res[1];
	position->z = res[2];
	*success = parse_success;
}

/**
 * @brief Parses given orientation into a vector
 * @param position Pointer to a vector that will be filled with orientation
 * @param str Raw string to be parsed
 * @param success Boolean pointer that will be set to false on error
 */
void	parse_orientation(t_vector *orientation, const char *str, bool *success)
{
	double	res[3];
	char	**coords;
	size_t	i;
	bool	parse_success;

	parse_success = true;
	coords = ft_split(str, ',');
	if (coords == NULL || count_commas(str) != 2 || split_count(coords) != 3)
	{
		*success = false;
		free_split_array(coords);
		return ;
	}
	i = -1;
	while (coords[++i] != NULL)
	{
		res[i] = ft_atof(coords[i], success);
		if (!is_num(coords[i], true) || !*success || res[i] < -1 || res[i] > 1)
			parse_success = false;
	}
	free_split_array(coords);
	orientation->x = res[0];
	orientation->y = res[1];
	orientation->z = res[2];
	*success = parse_success;
}
