/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_config_structure.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdal-mol <pdal-mol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 16:46:41 by pdal-mol          #+#    #+#             */
/*   Updated: 2022/11/14 15:31:06 by pdal-mol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

t_bool	str_is_in_charset(char *str, char *charset)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (!is_in_charset(str[i], charset))
			return (false);
		i++;
	}
	return (true);
}

static t_bool	check_structure(char **scene)
{
	int	i;

	i = 0;
	while ((is_config_line(scene[i])
			|| str_is_in_charset(scene[i], "\n")) && scene[i])
		i++;
	while (scene[i])
	{
		if (is_config_line(scene[i]))
			return (false);
		i++;
	}
	return (true);
}

t_bool	check_config_structure(char **scene)
{
	if (!check_structure(scene))
		return (false);
	return (true);
}
