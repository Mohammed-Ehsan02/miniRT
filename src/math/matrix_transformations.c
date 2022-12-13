/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_transformations.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 15:41:22 by mkhan             #+#    #+#             */
/*   Updated: 2022/12/06 23:44:16 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/**
 * @brief Initialize the translation matrix.
 * @param mat Matrix to be initialized.
 * @param x The value along the x-axis to be translated.
 * @param y The value along the y-axis to be translated.
 * @param z The value along the z-axis to be translated.
 */

void	translate_matrix(t_mat4 *mat, double x, double y, double z)
{
	ft_bzero(mat, sizeof(t_mat4));
	(*mat)[0][0] = 1;
	(*mat)[1][1] = 1;
	(*mat)[2][2] = 1;
	(*mat)[3][3] = 1;
	(*mat)[0][3] = x;
	(*mat)[1][3] = y;
	(*mat)[2][3] = z;
}

/**
 * @brief Initialize the scaling matrix
 * @param mat The matrix to be initialized
 * @param x The value along the x-axis to be scaled.
 * @param y The value along the y-axis to be scaled.
 * @param z The value along the z-axis to be scaled.
 */
void	scaling_matrix(t_mat4 *mat, double x, double y, double z)
{
	ft_bzero(mat, sizeof(t_mat4));
	(*mat)[0][0] = x;
	(*mat)[1][1] = y;
	(*mat)[2][2] = z;
	(*mat)[3][3] = 1;
}

/**
 * @brief Initialize the matrix for rotation along x-axis
 * @param mat The matrix to be initialized
 * @param r angle in radians
 */
void	rotation_matrix_x(t_mat4 *mat, double r)
{
	ft_bzero(mat, sizeof(t_mat4));
	(*mat)[0][0] = 1;
	(*mat)[1][1] = cos(r);
	(*mat)[1][2] = sin(r) * -1;
	(*mat)[2][1] = sin(r);
	(*mat)[2][2] = cos(r);
	(*mat)[3][3] = 1;
}

/**
 * @brief Initialize the matrix for rotation along y-axis
 * @param mat The matrix to be initialized
 * @param r angle in radians
 */
void	rotation_matrix_y(t_mat4 *mat, double r)
{
	ft_bzero(mat, sizeof(t_mat4));
	(*mat)[0][0] = cos(r);
	(*mat)[0][2] = sin(r);
	(*mat)[1][1] = 1;
	(*mat)[2][0] = sin(r) * -1;
	(*mat)[2][2] = cos(r);
	(*mat)[3][3] = 1;
}

/**
 * @brief Initialize the matrix for rotation along z-axis
 * @param mat The matrix to be initialized
 * @param r angle in radians
 */
void	rotation_matrix_z(t_mat4 *mat, double r)
{
	ft_bzero(mat, sizeof(t_mat4));
	(*mat)[0][0] = cos(r);
	(*mat)[0][1] = sin(r) * -1;
	(*mat)[1][0] = sin(r);
	(*mat)[1][1] = cos(r);
	(*mat)[2][2] = 1;
	(*mat)[3][3] = 1;
}

/**
 * @brief Convert Radians to Degrees
 * @param r The radian value
 * @return double The converted value in degrees
 */
double	rad_to_deg(double r)
{
	return (r * (180 / M_PI));
}

/**
 * @brief Convert Degrees to Radians
 * @param r The degree value
 * @return double The converted value in radians
 */
double	deg_to_rad(double r)
{
	return (r * (M_PI / 180));
}

void	axis_angle(t_mat4 *rot_mat, const t_vector *ax, double angle)
{
	(*rot_mat)[0][0] = cos(angle) + ax->x * ax->x * (1 - cos(angle));
	(*rot_mat)[0][1] = ax->x * ax->y * (1 - cos(angle)) - ax->z * sin(angle);
	(*rot_mat)[0][2] = ax->x * ax->z * (1 - cos(angle)) + ax->y * sin(angle);
	(*rot_mat)[0][3] = 0;
	(*rot_mat)[1][0] = ax->x * ax->y * (1 - cos(angle)) + ax->z * sin(angle);
	(*rot_mat)[1][1] = cos(angle) + ax->y * ax->y * (1 - cos(angle));
	(*rot_mat)[1][2] = ax->y * ax->z * (1 - cos(angle)) - ax->x * sin(angle);
	(*rot_mat)[1][3] = 0;
	(*rot_mat)[2][0] = ax->z * ax->x * (1 - cos(angle)) - ax->y * sin(angle);
	(*rot_mat)[2][1] = ax->z * ax->y * (1 - cos(angle)) + ax->x * sin(angle);
	(*rot_mat)[2][2] = cos(angle) + ax->z * ax->z * (1 - cos(angle));
	(*rot_mat)[2][3] = 0;
	(*rot_mat)[3][0] = 0;
	(*rot_mat)[3][1] = 0;
	(*rot_mat)[3][2] = 0;
	(*rot_mat)[3][3] = 1;
}

void	calculate_camera_transform(t_scene *scene)
{
	t_vector		from;
	t_vector		up;

	from.x = scene->camera.position.x;
	from.y = scene->camera.position.y;
	from.z = scene->camera.position.z;
	from.w = 1;
	normalize_vec(&scene->camera.orientation);
	up.x = 0;
	up.y = 1;
	up.z = 0;
	up.w = 0;
	view_transform(&scene->camera.transform, &from, &up,
		&scene->camera.orientation);
	mat_inverse(&scene->camera.inv_trans, &scene->camera.transform);
}

void	calculate_plane_rotation(t_mat4 *rot_transform, t_shape *plane)
{
	t_vector	up;
	t_vector	ax;
	double		angle;

	ax.w = 0;
	up.x = 0;
	up.y = 1;
	up.z = 0;
	up.w = 0;
	normalize_vec(&plane->orientation);
	cross_product(&ax, &plane->orientation, &up);
	angle = acos(dot_product(&plane->orientation, &up));
	axis_angle(rot_transform, &ax, angle);
}

void	multiply_transforms(t_shape *shape, t_mat4 *scale, t_mat4 *rot,
	t_mat4 *translate)
{
	mat_multiply(&shape->transf, translate, rot);
	ft_memcpy(translate, &shape->transf, sizeof(t_mat4));
	mat_multiply(&shape->transf, translate, scale);
	mat_inverse(&shape->inv_transf, &shape->transf);
	ft_memcpy(&shape->norm_transf, &shape->inv_transf, sizeof(t_mat4));
	transpose_matrix(&shape->norm_transf);
}

/**
 * @brief Calculates the transformation matrices for every object in the scene
 * The chosen transformation order is scale -> rotation -> translation
 * @param scene The scene struct containing the objects
 */
void	calculate_transforms(t_scene *scene)
{
	unsigned int	i;
	t_mat4			scale;
	t_mat4			rot;
	t_mat4			translate;

	calculate_camera_transform(scene);
	i = 0;
	while (i < scene->count.shape_count)
	{
		identity_matrix(&scene->shapes[i].transf);
		identity_matrix(&scale);
		identity_matrix(&rot);
		identity_matrix(&translate);
		if (scene->shapes[i].type == SPHERE)
			scaling_matrix(&scale, scene->shapes[i].radius,
				scene->shapes[i].radius, scene->shapes[i].radius);
		if (scene->shapes[i].type == PLANE)
			calculate_plane_rotation(&rot, &scene->shapes[i]);
		translate_matrix(&translate, scene->shapes[i].origin.x,
			scene->shapes[i].origin.y, scene->shapes[i].origin.z);
		multiply_transforms(&scene->shapes[i], &scale, &rot, &translate);
		i++;
	}
}
