#include "cubd3.h"
#include <stdio.h>

static void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*str;

	if (!s)
		return (NULL);
	i = 0;
	str = (unsigned char *)s;
	while (i < n)
	{
		str[i] = (unsigned char)c;
		i++;
	}
	return ((void *)str);
}

int main(int argc, char **argv)
{
    t_game  game;

    ft_memset(&game, 0, sizeof(t_game));
    if (argc != 2)
    {
        printf("Usage: ./cub3D <map.cub>\n");
        return (1);
    }
    
    if (!parse_scene(argv[1], &game.scene))
    {
        printf("Error\n%s\n", get_parse_error(&game.scene));
        cleanup_scene(&game.scene);
        return (1);
    }
    
    printf("OK: parsing passed\n");
    printf("OK spawn=(%d,%d,%c) rows=%d cols=%d\n",
           game.scene.spawn.row, game.scene.spawn.col, game.scene.spawn.dir,
           game.scene.map.rows, game.scene.map.cols);
    
    if (!init_game(&game))
    {
        printf("Error\nFailed to initialize game\n");
        cleanup_scene(&game.scene);
        return (1);
    }
    ft_memset(&game.keys, 0, sizeof(t_keys));
    mlx_hook(game.win, 2, 1L<<0, key_press, &game);
    mlx_hook(game.win, 3, 1L<<1, key_release, &game);
    mlx_hook(game.win, 17, 0, close_window, &game);
    mlx_loop_hook(game.mlx, game_loop, &game);
    mlx_loop(game.mlx);
    
    // (Jamais atteint avec mlx_loop)
    cleanup_mlx(&game);
    cleanup_scene(&game.scene);
    return (0);
}
