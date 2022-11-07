/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdal-mol <pdal-mol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 11:52:43 by pdal-mol          #+#    #+#             */
/*   Updated: 2022/11/07 15:31:05 by pdal-mol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

float	get_ray_len(t_data *data, t_ray *ray)
{
	return (
		sqrtf(((ray->y_end - data->player->y) * (ray->y_end - data->player->y))
			+ ((ray->x_end - data->player->x) * (ray->x_end - data->player->x)))
	);
}

void	draw_rays(t_data *data)
{
	t_ray	*ray;
	float	temp;
	float	i;

	temp = data->player->pov;
	i = 0;
	data->player->pov += data->precomputed->fov_amplitude;
	while (i < FOV)
	{
		data->player->pov -= FOV_STEP;
		if (data->player->pov < 0)
			data->player->pov += TWO_PI;
		if (data->player->pov > TWO_PI)
			data->player->pov -= TWO_PI;
		ray = get_collision_coord(data, data->player->pov);
		draw_line(data, ray->x_end, ray->y_end);
		free(ray);
		i++;
	}
	data->player->pov = temp;
}

int	get_pixel_from_sprite_y(t_data *data, float wall_height, int wall_cursor_y)
{
	int	pixel_y_sprite;

	// pixel_y_sprite = wall_height / SPRITE_SIZE * wall_cursor_y;
	pixel_y_sprite = (wall_cursor_y - (data->precomputed->float_line - wall_height * 0.5)) / wall_height * SPRITE_SIZE;
	// printf("ratio pixel y : %f\n", (wall_cursor_y - (FLOAT_LINE - wall_height / 2)));
	return (pixel_y_sprite);
}

int	get_pixel_from_sprite_x(t_data *data, float pov)
{
	t_ray	*ray;
	int		pixel_x_sprite;

	ray = get_collision_coord(data, pov);

	if (ray->dir == NO || ray->dir == SO)
		pixel_x_sprite = (ray->x_end - floor(ray->x_end)) * SPRITE_SIZE; // attention si valur deja ronde pour ray->x_end floor en dessous voir si mieux dutiliser cast (int)
	else
		pixel_x_sprite = (ray->y_end - floor(ray->y_end)) * SPRITE_SIZE; 
	free(ray);
	return (pixel_x_sprite);
}

int ft_get_color_from_texture(t_texture *texture, int x, int y)
{
    return (*(int *)(texture->addr + (y * texture->line_length + x * texture->bits_per_pixel / 8)));
}

void	put_stripe_to_image(t_data *data, float wall_height_coef,
		int stripe_index, int wall_dir, float pov)
{
	float	wall_height;
	int		pixel_x;
	int		pixel_y;
	int		color;

	wall_height = wall_height_coef * WIN_HEIGHT;
	if (wall_height > WIN_HEIGHT)
		wall_height = WIN_HEIGHT;
	pixel_x = stripe_index * STRIPE;

	pixel_y = data->precomputed->float_line - (wall_height * 0.5);

	// printf("divide by two = %f | bitshift %f\n",(wall_height * 0.5), (float)((long)wall_height >> 1));
	int x_max = stripe_index * data->precomputed->stripe + data->precomputed->stripe + 1;
	int	y_max = data->precomputed->float_line + wall_height * 0.5;
	float	half_wall_height = wall_height * 0.5;
	color = 0x00000;
	while (pixel_x < x_max)
	{
		pixel_y = data->precomputed->float_line - half_wall_height;
		while (pixel_y < y_max)
		{
			if (pixel_y % 2 == 0 )
				color = ft_get_color_from_texture(data->textures[wall_dir], get_pixel_from_sprite_x(data, pov), get_pixel_from_sprite_y(data, wall_height, pixel_y));
			my_mlx_pixel_put(data->image, pixel_x, pixel_y, color);
			pixel_y++;
		}
		pixel_x++;
	}
}

void	get_wall_height(t_data *data)
{
	t_ray	*ray;
	float	mid_ray;
	float	wall_height_coef;
	float	ray_len;
	float	i;

	i = 0;
	mid_ray = data->player->pov;
	// data->player->pov += data->precomputed->fov_amplitude;
	float pov = mid_ray + data->precomputed->fov_amplitude;
	while (i < FOV)
	{
		// printf("pov:%f\n", pov);
		if (pov < 0)
			pov += TWO_PI;
		if (pov > TWO_PI)
			pov -= TWO_PI;
		ray = get_collision_coord(data, pov);
		ray_len = get_ray_len(data, ray)
			* cos(fabs(pov - mid_ray));
		wall_height_coef = 1 / ray_len;
		put_stripe_to_image(data, wall_height_coef, i, ray->dir, pov);
		free(ray);
		pov -= FOV_STEP;
		i++;
	}
}

void	raycasting(t_data *data)
{
	get_wall_height(data);
	// draw_rays(data);
	mlx_put_image_to_window(data->mlx, data->mlx_win, data->image->ptr, 0, 0);
	// mlx_destroy_image(data->mlx, data->image)
}
