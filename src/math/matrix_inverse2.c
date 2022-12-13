/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_inverse2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 17:01:41 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/02 21:56:59 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/**
 * @brief Returns the cofactor of a matrix at a mat[row][col]
 * @param mat4 4x4 matrix to find the cofactor of
 * @param row Row of the cofactor
 * @param col Column of the cofactor
 * @return The cofactor at mat[row][col]
 */
static double	cofactor4(const t_mat4 *mat4, int row, int col)
{
	t_mat3	mat3;

	submat4(&mat3, mat4, row, col);
	if ((row + col) % 2 == 1)
		return (det3(&mat3));
	return (-det3(&mat3));
}

/**
 * @brief Returns the determinant
 * @param mat4 Matrix to find the determinant of
 * @return The determinant of a 4 by 4 matrix
 */
static double	det4(const t_mat4 *mat4)
{
	return (cofactor4(mat4, 0, 0) * (*mat4)[0][0]
			+ cofactor4(mat4, 0, 1) * (*mat4)[0][1]
			+ cofactor4(mat4, 0, 2) * (*mat4)[0][2]
			+ cofactor4(mat4, 0, 3) * (*mat4)[0][3]);
}

/**
 * @brief Inverts a 4x4 matrix
 * @param res This matrix will be set to the inverse of mat
 * @param mat The matrix to invert
 */
void	mat_inverse(t_mat4 *res, const t_mat4 *mat)
{
	double	det;
	int		row;

	det = det4(mat);
	if (fabs(det) < 0.01)
		return ;
	row = 0;
	while (row < 4)
	{
		(*res)[0][row] = cofactor4(mat, row, 0) / det;
		(*res)[1][row] = cofactor4(mat, row, 1) / det;
		(*res)[2][row] = cofactor4(mat, row, 2) / det;
		(*res)[3][row] = cofactor4(mat, row, 3) / det;
		row++;
	}
}
