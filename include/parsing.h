/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 13:45:41 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/12 11:43:40 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "miniRT.h"

# define RED "\x1b[31m"
# define RESET "\x1b[0m"
# define YELLOW "\x1b[33m"
# define MAGENTA "\e[0;35m"
# define LIGHT_MAX 20
# define SHAPE_MAX 100

// parse_scene.c
bool	skip_line(char **line, int fd, size_t *line_count);
t_scene	*parse_scene(int fd);

// parse_attributes.c
bool	check_orientation(const t_vector *orientation, size_t line_num,
			const char *line, const char *element);
bool	check_color(const t_color *color, size_t line_num, const char *line,
			const char *element);
void	parse_color(t_color *color, const char *str, bool *success);
void	parse_coordinates(t_vector *position, const char *str, bool *success);
void	parse_orientation(t_vector *orientation, const char *str,
			bool *success);

// parse_elements.c
bool	parse_light(t_scene *scene, char **splitted, char *line,
			size_t line_num);
bool	parse_ambient(t_scene *scene, char **splitted, size_t line_num,
			char *line);
bool	parse_camera(t_scene *scene, char **splitted, char *line,
			size_t line_num);

// parse_shapes.c
bool	is_shape(const char *identifier);
bool	parse_shape(t_scene *scene, char **splitted, size_t line_num,
			char *line);

// parse_utils.c
bool	all_whitespace(const char *str);
size_t	count_commas(const char *str);
bool	is_num(const char *str, bool decimal);
size_t	split_count(char **split);

// shape_errors.c
void	*shape_parse_error(char *line, size_t line_count, t_scene *scene,
			char **splitted);

// scene_errors.c
void	*camera_parse_error(char *line, size_t line_num, t_scene *scene,
			bool invalid_coords);
void	*ambient_parse_error(char *line, size_t line_num, t_scene *scene);
void	*unknown_identifier(char *line, size_t line_num, t_scene *scene,
			char **splitted);
void	*light_parse_error(char *line, size_t line_count, t_scene *scene);

// parse_settings.c
bool	is_settings(const char *line);
bool	parse_settings(t_scene *scene, const char *line, size_t line_num, int fd);

#endif