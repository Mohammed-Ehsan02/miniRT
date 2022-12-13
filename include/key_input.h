/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_input.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 19:44:18 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/11 12:58:08 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEY_INPUT_H
# define KEY_INPUT_H

# include "scene.h"
# define ON_DESTROY 17
# define PI           3.14159265358979323846F
# define KEY_ESC 53
# define L_KEY_ESC 65307
# define KEY_W 13
# define L_KEY_W 119
# define KEY_A 0
# define L_KEY_A 97
# define KEY_S 1
# define L_KEY_S 115
# define KEY_D 2
# define L_KEY_D 100
# define KEY_LEFT 123
# define L_KEY_LEFT 65361
# define KEY_RIGHT 124
# define L_KEY_RIGHT 65363
# define KEY_UP 126
# define L_KEY_UP 65362
# define KEY_DOWN 125
# define L_KEY_DOWN 65364
# define KEY_Q 12
# define L_KEY_Q 113
# define KEY_E 14
# define L_KEY_E 101
# define KEY_PLUS 24
# define L_KEY_PLUS 61
# define KEY_MINUS 27
# define L_KEY_MINUS 45
# define KEY_P 35
# define L_KEY_P 112
# define KEY_C 8
# define L_KEY_C 99
# define KEY_TAB 48
# define KEY_SPACE 49
# define KEY_R 15

int	key_handler(t_scene *scene);

#endif