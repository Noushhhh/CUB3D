/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdal-mol <pdal-mol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 13:00:31 by pdal-mol          #+#    #+#             */
/*   Updated: 2022/11/07 15:42:37 by pdal-mol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static t_bool	check_collision_y(t_data *data, t_ray *ray, float pov)
{
	if (ray->y_end == 0 || ray->y_end == data->y_max)
		return (true);
	if (pov > 0 && pov <= PI_ON_TWO)
		if (data->map[(int)(ray->y_end - 1)][(int)floor(ray->x_end)] == '1')
			return (true);
	if (pov >= PI_ON_TWO && pov < PI)
		if (data->map[(int)(ray->y_end - 1)][(int)floor(ray->x_end)] == '1')
			return (true);
	if (pov > PI && pov <= THREE_PI_ON_TWO)
		if (data->map[(int)(ray->y_end)][(int)floor(ray->x_end)] == '1')
			return (true);
	if (pov > THREE_PI_ON_TWO)
		if (data->map[(int)(ray->y_end)][(int)floor(ray->x_end)] == '1')
			return (true);
	return (false);
}

static t_bool	check_collision_x(t_data *data, t_ray *ray, float pov)
{
	if (pov == 0 || pov == PI)
		if (data->map[(int)ray->y_end][(int)ray->x_end] == '1')
			return (true);
	if (ray->x_end == 0 || ray->x_end
		== data->precomputed->map_lines_len[(int)floor(ray->y_end)])
		return (true);
	if (pov >= 0 && pov <= PI_ON_TWO)
		if (data->map[(int)floor(ray->y_end)][(int)(ray->x_end)] == '1')
			return (true);
	if (pov > PI_ON_TWO && pov <= M_PI)
		if (data->map[(int)floor(ray->y_end)][(int)(ray->x_end - 1)] == '1')
			return (true);
	if (pov >= PI && pov <= THREE_PI_ON_TWO)
		if (data->map[(int)floor(ray->y_end)][(int)(ray->x_end - 1)] == '1')
			return (true);
	if (pov > THREE_PI_ON_TWO)
		if (data->map[(int)floor(ray->y_end)][(int)(ray->x_end)] == '1')
			return (true);
	return (false);
}


static t_ray	*get_collision_y(t_data *data, float pov)
{
	t_ray	*ray;

	ray = malloc(sizeof(t_ray));
	if (!ray)
		error(MEMALLOC);
	ray->x_end = 1000000;
	ray->y_end = data->player->y;
	ray->angle = pov;
	ray->coll = false;
	while (!ray->coll)
	{
		ray->y_end = get_fixed_ray_end(ray, 'y', pov);
		ray->dir = get_wall_dir(ray, 'y', pov);
		if (pov == PI || pov == 0)
			return (ray);
		ray->x_end = get_x_with_y(data, ray->y_end, pov);
		if (ray->x_end >= data->precomputed->map_lines_len[(int)ray->y_end]
			|| ray->x_end < 0)
			return (ray);
		if (ray->y_end > data->y_max || ray->y_end < 0)
			return (ray);
		if (check_collision_y(data, ray, pov))
			ray->coll = true;
		
	}
	return (ray);
}

static t_ray	*get_collision_x(t_data *data, float pov)
{
	t_ray	*ray;

	ray = malloc(sizeof(t_ray));
	if (!ray)
		error(MEMALLOC);
	ray->x_end = data->player->x;
	ray->y_end = 1000000;
	ray->angle = pov;
	ray->coll = false;
	while (!ray->coll)
	{
		ray->x_end = get_fixed_ray_end(ray, 'x', pov);
		ray->dir = get_wall_dir(ray, 'x', pov);
		if (pov == PI_ON_TWO || pov == THREE_PI_ON_TWO)
			return (ray);
		ray->y_end = get_y_with_x(data, ray->x_end, pov);
		if (ray->y_end >= data->y_max || ray->y_end < 0)
			return (ray);
		if (ray->x_end >= data->precomputed->map_lines_len[(int)ray->y_end]
			|| ray->x_end < 0)
			return (ray);
		if (check_collision_x(data, ray, pov))
			ray->coll = true;
	}
	return (ray);
}

t_ray	*get_collision_coord(t_data *data, float pov)
{
	t_ray	*ray_horizontal;
	t_ray	*ray_vertical;


	ray_vertical = get_collision_x(data, pov);
	ray_horizontal = get_collision_y(data, pov);
	ray_vertical->len = get_ray_len(data, ray_vertical);
	ray_horizontal->len = get_ray_len(data, ray_horizontal);
	if (ray_horizontal->len < ray_vertical->len)
	{
		free(ray_vertical);
		return (ray_horizontal);
	}
	free(ray_horizontal);
	return (ray_vertical);
}
