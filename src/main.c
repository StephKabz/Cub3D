#include "cubd3.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    t_game  game;
    
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
    raycasting(&game);
    mlx_put_image_to_window(game.mlx, game.win, game.img, 0, 0);
    mlx_loop(game.mlx);
    
    // (Jamais atteint avec mlx_loop)
    cleanup_mlx(&game);
    cleanup_scene(&game.scene);
    return (0);
}
