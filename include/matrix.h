/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 10:38:18 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/23 12:03:13 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATRIX_H
# define MATRIX_H

# include "miniRT.h"

typedef float			t_mat4[4][4];
typedef float			t_mat3[3][3];
typedef float			t_mat2[2][2];

typedef struct s_scene	t_scene;

// matrix_operations.c
void	mat_multiply(t_mat4 *res, const t_mat4 *m1, const t_mat4 *m2);
void	mat_vec_multiply(t_vector *res, const t_mat4 *mat,
			const t_vector *vec);
void	transpose_matrix(t_mat4 *mat);
void	translate_matrix(t_mat4 *mat, float x, float y, float z);
void	scaling_matrix(t_mat4 *mat, float x, float y, float z);

// matrix_inverse.c
float	det3(const t_mat3 *mat3);
void	submat4(t_mat3 *res, const t_mat4 *mat, int row, int col);
void	mat_inverse(t_mat4 *res, const t_mat4 *mat);

// matrix_transformations.c
void	calculate_transforms(t_scene *scene);
void	identity_matrix(t_mat4 *mat);

void	rotation_matrix_x(t_mat4 *mat, float r);
void	rotation_matrix_y(t_mat4 *mat, float r);
void	rotation_matrix_z(t_mat4 *mat, float r);
void	axis_angle(t_mat4 *rot_mat, const t_vector *ax, float angle);

// ! Wont be submitted
void	print_mat4(const t_mat4 *mat);
void	print_mat3(const t_mat3 *mat);
void	print_mat2(const t_mat2 *mat);
#endif