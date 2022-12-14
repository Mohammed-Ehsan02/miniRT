/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 13:45:41 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/06 16:16:27 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# define RED "\x1b[31m"
# define RESET "\x1b[0m"
# define YELLOW "\x1b[33m"
# define MAGENTA "\e[0;35m"
# define GREEN "\x1B[32m"
# define LIGHT_MAX 20
# define SHAPE_MAX 100

typedef struct s_color_error	t_color_error;
struct s_color_error
{
	bool	r;
	bool	g;
	bool	b;
	bool	other;
};

typedef struct s_orient_error	t_orient_error;
struct s_orient_error
{
	bool	x;
	bool	y;
	bool	z;
	bool	zero;
	bool	other;
};

typedef struct s_ambient_errors	t_ambient_errors;
struct s_ambient_errors
{
	bool			intensity_other;
	bool			intensity_range;
	bool			other;
	t_color_error	color;
};

typedef struct s_cam_errors	t_cam_errors;
struct s_cam_errors
{
	bool			other;
	bool			coords;
	bool			fov_other;
	bool			fov_range;
	bool			up_vector;
	t_orient_error	dir;
};

typedef struct s_light_errors	t_light_errors;
struct s_light_errors
{
	bool			intensity_other;
	bool			intensity_range;
	bool			max_lights;
	bool			other;
	bool			coords;
	t_color_error	color;
};

typedef struct s_shape_errors	t_shape_errors;
struct s_shape_errors
{
	bool			max_shapes;
	bool			diameter_range;
	bool			diameter_other;
	bool			height_range;
	bool			height_other;
	bool			origin;
	bool			side_len_other;
	bool			side_len_range;
	bool			other;
	t_color_error	color;
	t_orient_error	orient;
};

typedef struct s_parse_error_flags	t_error_flags;
struct	s_parse_error_flags
{
	t_ambient_errors	ambient;
	t_cam_errors		cam;
	t_light_errors		light;
	t_shape_errors		shape;
	bool				unknown_identifier;
	bool				settings_err;
};

bool	find_error(t_error_flags *errors);

// Error with parsing these elements
// ambient/camera/camera fov/light intensity/sphere diameter/sphere/plane/cylinder/
// cylinder height/cylinder diameter/cube/cube side length/shape
# define GENERIC_ERROR YELLOW"Error with parsing %s on line #%d\n\n"RED"->\t%s\n\n"

// orientation errors
# define ORIENT_X_OOR "\x1b[33mThe x value is out of range [-1.0 -> 1.0]\n\x1b[0m"
# define ORIENT_Y_OOR "\x1b[33mThe y value is out of range [-1.0 -> 1.0]\n\x1b[0m"
# define ORIENT_Z_OOR "\x1b[33mThe z value is out of range [-1.0 -> 1.0]\n\x1b[0m"
# define ORIENT_ZERO "\x1b[33mThe orientation vector cannot be the zero vector\n\x1b[0m"

// color errors
# define RED_OOR YELLOW"The red value is out of range [0 -> 255]\n"RESET
# define GREEN_OOR YELLOW"The green value is out of range [0 -> 255]\n"RESET
# define BLUE_OOR YELLOW"The blue value is out of range [0 -> 255]\n"RESET

// multi errors (multiple cameras/ multiple ambient lights)
# define MULTI_ELEMENT_ERROR RED"Error: Scene contains multiple %s\n"RESET

// no errors (no cameras/ no ambient lights)
# define NO_ELEMENT_ERROR RED"Error: Scene contains no %s\n"RESET

// max element error (> MAX_LIGHTS or > MAX_SHAPES)
# define MAX_ELEMENT_ERROR RED"Error: Scene contains more than %d %s\n"RESET

# define UNKNOWN_IDENTIFIER YELLOW"Unknown identifier \"%s\" on line #%d\n"RED"->\t%s\n"RESET

// Ambient intensity out of range/ light intensity out of range
# define LIGHT_INTENSITY_OOR YELLOW"%s intensity value is out of range on line #%d\n\n"RED"->\t%s\n\n"RESET

# define CAMERA_UP_VECTOR YELLOW"Camera orientation cannot be the up vector (0, 1, 0)\n"RESET
# define CAMERA_FOV_OOR YELLOW"The fov value is out of range [1 -> 180]\n"RESET

// diameter/side length/height
# define POSITIVE_VALUE YELLOW"%s has to be a positive number on line #%d\n\n"RED"->\t%s\n\n"RESET

# define LIGHT_SYNTAX YELLOW"Correct syntax is \"L [origin] [intensity] [color]\"\n"RESET
# define AMBIENT_LIGHT_SYNTAX YELLOW"Correct syntax is \"A [intensity] [color]\"\n"RESET
# define CAMERA_SYNTAX YELLOW"Correct syntax is \"C [origin] [orientation] [fov]\"\n"RESET
# define SPHERE_SYNTAX YELLOW"Correct syntax is \"sp [origin] [diameter] [color]\"\n"RESET
# define PLANE_SYNTAX YELLOW"Correct syntax is \"pl [origin] [orientation] [color]\"\n"RESET
# define CYLINDER_SYNTAX YELLOW"Correct syntax is \"cy [origin] [orientation] [diameter] [height] [color]\"\n"RESET
# define CONE_SYNTAX YELLOW"Correct syntax is \"co [origin] [orientation] [diameter] [height] [color]\"\n"RESET
# define CUBE_SYNTAX YELLOW"Correct syntax is \"cu [origin] [side length] [color]\"\n"RESET

# define SETTINGS_NO_SHAPE RED"Settings at line %ld do not belong to any shape\n"RESET

# define UNTERMINATED_SETTINGS RED"Unterminated shape settings starting at line %ld\n"RESET

# define EMPTY_SETTINGS RED"Empty shape settings starting at line %ld\n"RESET

# define EXTRA_OPENING_BRACE RED"Shape settings starting at line %ld contains an extra opening brace\n"RESET

# define EXTRA_CLOSING_BRACE RED"Shape settings starting at line %ld contains an extra closing brace\n"RESET

# define INVALID_TERMINATION RED"Shape settings starting at line %ld is not terminated correctly\n"RESET

# define INVALID_PROPERTY YELLOW"Error with parsing this property\n"RED"->\t%s\n"YELLOW"Correct syntax is KEY : VALUE\n"RESET

# define INVALID_KEY YELLOW"Error with parsing this property\n"RED"->\t%s\n"YELLOW"`%s` is not a valid key\n"RESET

# define COLOR_SETTING_ERROR YELLOW"Error with parsing this property\n"RED"->\t%s : %s\n"YELLOW"`%s` is not a valid value\n"MAGENTA"Available colors are BLUE, RED, PURPLE, GREEN, YELLOW, PINK, BLACK, GRAY\n"RESET

# define INVALID_PROPERTY_VALUE YELLOW"Error with parsing this property\n"RED"->\t%s : %s\n"YELLOW"`%s` is not a valid value\n"RESET

# define INVALID_PROPERTY_RANGE YELLOW"Error with parsing this property\n"RED"->\t%s : %s\n"YELLOW"%s has to be between %.2f and %.2f\n"RESET

bool	parse_ambient(t_scene *scene, char **splitted);
void	parse_camera(t_scene *scene, char **splitted);
void	parse_light(t_scene *scene, char **splitted);

bool	parse_shape(t_scene *scene, char **splitted);

bool	parse_settings(t_scene *scene, const char *settings_start,
			size_t *line_num, int fd);

void	parse_coordinates(t_vector *position, const char *str, bool *success);
void	parse_orientation(t_vector *orientation, const char *str, t_orient_error *err);

void	parse_color(t_color *color, const char *str, t_color_error *errs);

bool	is_settings(const char *line);
bool	is_shape(const char *identifier);

bool	is_num(const char *str, bool decimal);
size_t	count_commas(const char *str);
size_t	split_count(char **split);
bool	all_whitespace(const char *str);
void	print_error(t_scene *scene, const char *line, int line_num,
			const char *identifer);
t_scene	*parse_scene(int fd);
void	init_shape(t_shape *shape, t_scene *scene);
void	parse_sphere(t_scene *scene, t_shape *shape, char **splitted);
void	parse_cube(t_scene *scene, t_shape *shape, char **splitted);
void	parse_plane(t_scene *scene, t_shape *shape, char **splitted);
bool	print_shape_error(t_shape *shape, t_shape_errors *err, const char *line,
			int line_num);
bool	print_color_error(t_color_error *err, const char *line, int line_num,
			char *element);
bool	print_orient_error(t_orient_error *err, const char *line, int line_num,
			char *element);
bool	print_ambient_light_error(t_ambient_errors *err, const char *line,
			int line_num);
			
bool	print_cam_error(t_cam_errors *err, const char *line, int line_num);
bool	print_light_error(t_light_errors *err, const char *line, int line_num);
bool	print_sphere_error(t_shape_errors *err, const char *line, int line_num);
bool	print_cube_error(t_shape_errors *err, const char *line, int line_num);
bool	print_plane_error(t_shape_errors *err, const char *line, int line_num);
bool	print_cylinder_error(t_shape_errors *err, const char *line,
		int line_num);
bool	print_cone_error(t_shape_errors *err, const char *line, int line_num);

bool	is_settings(const char *line);
bool	is_valid_key(const char *key);
bool	is_valid_color(const char *color);
bool	check_value(const char *key, const char *val, float min, float max);
bool	is_valid_val(const char *key, const char *val);
t_color	parse_color_value(const char *str);
void	parse_setting(t_shape *shape, char **key_val);
bool	check_braces(char *settings_str, size_t *line_num);
char	*check_settings_str(char *settings_str, size_t *line_num);
char	*get_settings_str(size_t *line_num, int fd, const char *settings_start);
bool	check_colons(char *line);
bool	parse_split_settings(t_scene *scene, char **settings);
bool	parse_settings(t_scene *scene, const char *settings_start, size_t *line_num, int fd);

#endif
