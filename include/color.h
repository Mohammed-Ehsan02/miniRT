/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 14:04:11 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/17 15:05:51 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLOR_H
# define COLOR_H

// This might not be necessary

/**
 * @brief Represents a color. (This could be represented as 4 chars or even as
 *  1 int)
 * @param r Red
 * @param g Green
 * @param b Blue
 * @param a Transparency
 */
typedef struct s_color	t_color;
struct s_color
{
	float	r;
	float	g;
	float	b;
	float	a;
};
#endif
