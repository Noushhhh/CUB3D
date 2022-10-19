#include "../../includes/cub3d.h"

char    get_player_token(t_data *data)
{
    size_t	i;
	size_t	j;

	i = 0;
	while (data->map[i])
	{
		j = 0;
		while (data->map[i][j])
		{
			if (is_in_charset(data->map[i][j], "NSEW"))
			{
                if (is_in_charset(data->map[i][j], "N"))
                    return ('N');
                else if (is_in_charset(data->map[i][j], "S"))
                    return ('S');
                else if (is_in_charset(data->map[i][j], "E"))
                    return ('E');
                return ('W');
			}
			j++;
		}
		i++;
	}
    return (0);
}

void	rotate_left(t_data **data)
{
	(*data)->player->pov -= 0.1 * ROT_COEF;
	if ((*data)->player->pov <= 0)
		(*data)->player->pov = (2 * M_PI - (0.1 * ROT_COEF));

}

void	rotate_right(t_data **data)
{
	(*data)->player->pov += 0.1 * ROT_COEF;
	if ((*data)->player->pov >= 2 * M_PI)
		(*data)->player->pov = (0 + (0.1 * ROT_COEF));
}
