/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 12:39:00 by mkhan             #+#    #+#             */
/*   Updated: 2022/12/12 13:52:12 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	reflect(t_vector *res, t_vector *in_vector, t_vector *normal)
{
	scale_vec(res, normal, dot_product(in_vector, normal) * 2);
	sub_vec(res, in_vector, res);
}

t_color	get_ambient(t_color *effective_color,
	t_light *light, t_scene *scene)
{
	t_color	ambient;

	mult_color(&ambient, effective_color,
		scene->ambient.intensity * light->intensity);
	blend_colors(&ambient, &ambient, &scene->ambient.color);
	return (ambient);
}

bool	get_specular_and_diffuse(t_scene *scene, int light_idx,
	t_intersect *itx, t_color *diffuse, t_color *effective_color,
	t_color *specular)
{
	double		reflect_dot_eye;
	double		light_dot_normal;
	t_vector	light_v;
	t_vector	reflect_v;

	sub_vec(&light_v, &scene->lights[light_idx].position, &itx->over_point);
	light_v.w = 0;
	normalize_vec(&light_v);
	itx->normal.w = 0;
	light_dot_normal = dot_product(&light_v, &itx->normal);
	if (light_dot_normal < 0 || is_shadowed(scene, light_idx, &itx->over_point))
		return (false);
	mult_color(diffuse, effective_color, itx->shape->diffuse * light_dot_normal
		* scene->lights[light_idx].intensity);
	negate_vec(&light_v, &light_v);
	reflect(&reflect_v, &light_v, &itx->normal);
	reflect_dot_eye = dot_product(&reflect_v, &itx->eye);
	if (reflect_dot_eye <= 0)
		ft_bzero(specular, sizeof(t_color));
	else
		mult_color(specular, &scene->lights[light_idx].color,
			itx->shape->specular * pow(reflect_dot_eye, itx->shape->shininess)
			* scene->lights[light_idx].intensity);
	return (true);
}

t_color	lighting(t_intersect *itx, t_scene *scene, int light_idx)
{
	t_color		effective_color;
	t_color		diffuse;
	t_color		specular;
	t_color		result;
	// t_color		highlighted_color;
	
	// highlighted_color.a = 0;
	// highlighted_color.r = 0xff / 255.0;
	// highlighted_color.g = 0xde / 255.0;
	// highlighted_color.b = 0xeb / 255.0;
	// if (itx->shape->highlighted == true)
	// {
	// 	blend_colors(&effective_color, &highlighted_color,
	// 		&scene->lights[light_idx].color);
	// }
	// else
	// {
		blend_colors(&effective_color, &itx->shape->color,
			&scene->lights[light_idx].color);
	// }
	if (get_specular_and_diffuse(scene, light_idx, itx, &diffuse,
			&effective_color, &specular) == false)
		return (get_ambient(&effective_color,
				&scene->lights[light_idx], scene));
	result = get_ambient(&effective_color, &scene->lights[light_idx], scene);
	result.r += diffuse.r + specular.r;
	result.g += diffuse.g + specular.g;
	result.b += diffuse.b + specular.b;
	return (result);
}
