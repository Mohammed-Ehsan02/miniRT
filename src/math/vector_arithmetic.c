/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_arithmetic.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 09:46:11 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/04 18:35:49 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/**
 * @brief Adds two vectors together and stores the result in res
 * @param res The result of v1 + v2 will be stored in this vector
 * @param v1 First operand
 * @param v2 Second operand
 */
void	add_vec(t_vector *res, const t_vector *v1, const t_vector *v2)
{
	res->x = v1->x + v2->x;
	res->y = v1->y + v2->y;
	res->z = v1->z + v2->z;
	res->w = v1->w + v2->w;
}

/**
 * @brief Subtracts two vectors together and stores the result in res
 * @param res The result of v1 - v2 will be stored in this vector
 * @param v1 First operand
 * @param v2 Second operand
 */
void	sub_vec(t_vector *res, const t_vector *v1, const t_vector *v2)
{
	res->x = v1->x - v2->x;
	res->y = v1->y - v2->y;
	res->z = v1->z - v2->z;
	res->w = v1->w - v2->w;
}

/**
 * @brief Scales a vector and stores the result in res
 * @param res The result of scale * v will be stored in this vector
 * @param v Vector to be scaled
 * @param scale scaling factor
 */
void	scale_vec(t_vector *res, const t_vector *v, double scale)
{
	res->x = scale * v->x;
	res->y = scale * v->y;
	res->z = scale * v->z;
	res->w = scale * v->w;
}

/**
 * @brief Negates a vector and stores the result in res
 * @param res The result of -v will be stored in this vector
 * @param v Vector to be negated
 */
void	negate_vec(t_vector *res, const t_vector *v)
{
	res->x = -1 * v->x;
	res->y = -1 * v->y;
	res->z = -1 * v->z;
	res->w = -1 * v->w;
}
