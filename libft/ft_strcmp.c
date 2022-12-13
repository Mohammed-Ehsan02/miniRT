/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/20 16:12:51 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/12 10:39:11 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 != '\0' && *s2 != '\0')
	{
		if (*s1 > *s2)
			return (1);
		if (*s1 < *s2)
			return (-1);
		s1++;
		s2++;
	}
	if (*s1 != '\0')
		return (1);
	if (*s2 != '\0')
		return (-1);
	return (0);
}

int	ft_strcmp_case(const char *s1, const char *s2)
{
	while (ft_tolower(*s1) != '\0' && ft_tolower(*s2) != '\0')
	{
		if (ft_tolower(*s1) > ft_tolower(*s2))
			return (1);
		if (ft_tolower(*s1) < ft_tolower(*s2))
			return (-1);
		s1++;
		s2++;
	}
	if (*s1 != '\0')
		return (1);
	if (*s2 != '\0')
		return (-1);
	return (0);
}
