#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <limits.h>
# include <stdbool.h>
# include <stdint.h>
# include <mlx.h>
# include <mlx_int.h>

# define READ_BUFFER 4096
# define WIDTH 1000
# define HEIGHT 1000
# define MOVE_SPEED 0.5
# define ROT_SPEED 0.03

/* ---------- Types_parsing ---------- */

typedef struct s_textures {
    char *no;
    char *so;
    char *we;
    char *ea;
} t_textures;

typedef struct s_color{
    int red;
    int green;
    int blue;
    int defined;
} t_color;

typedef struct s_fc_colors {
    t_color floor;
    t_color ceiling;
} t_fc_colors;

typedef struct s_seen {
    int no;
    int so;
    int we;
    int ea;
    int f;
    int c;
} t_seen;

typedef struct s_map {
    char **lines_map;
    int   rows;
    int   cols;
} t_map;

typedef struct s_spawn {
    int   found;
    int   row;
    int   col;
    char  dir;
} t_spawn;

typedef struct s_scene {
    t_textures  textures;
    t_fc_colors fc;
    t_map       map;
    t_spawn     spawn;
    char       *err;
} t_scene;

typedef struct s_file_reader
{
	char	*data;
	size_t	capacity;
	size_t	length;
	t_scene	*scene;
}	t_file_reader;

/* ---------- Types_exec ---------- */

typedef struct s_player
{
    float   posX;
    float   posY;
    float   dirX;
    float   dirY;
    float   planeX;
    float   planeY;
} t_player;

typedef struct s_textures_img
{
    void    *img;
    char    *img_data;
    int     width;
    int     height;
    int     bpp;
    int     line_len;
    int     endian;
}   t_textures_img;

typedef struct s_all_textures
{
    t_textures_img  north;
    t_textures_img  south;
    t_textures_img  west;
    t_textures_img  east;
} t_all_textures;

typedef struct s_game
{
    void            *mlx;
    void            *win;
    void            *img;
    char            *img_data;
    int             bpp;
    int             line_len;
    int             endian;
    t_scene         scene;
    t_all_textures  textures;
    t_player        player;
}   t_game;

typedef struct s_ray
{
    double  ray_dir_x; //direction du rayon x
    double  ray_dir_y; //direction du rayon x
    int     map_x; // case x de la map
    int     map_y; // case y de la map
    double  side_dist_x; // disttance jusqu'au prochain cote de x
    double  side_dist_y; // disttance jusqu'au prochain cote de y
    double  delta_dist_x; // distance pour traverser 1 case en x
    double  delta_dist_y; // distance pour traverser 1 case en y
    int     step_x; // direction de step en x (+1 ou -1)
    int     step_y; // direction de step en y (+1 ou -1)
    int     hit; // variable si le mur est touche ou pas (1 ou 0)
    int     side; // cote touche (0 = verticale, 1 = horizontal)
    double  perp_dist_wall; // distance perpendiculaire au mur
}   t_ray;

/* ---------- Utils (src/utils/) ---------- */
char    *ft_strrchr(const char *s, int c);
int     ft_strncmp(const char *s1, const char *s2, size_t n);
size_t  ft_strlen(const char *s);
void    *ft_memcpy(void *dst, const void *src, size_t n);
int     ft_isdigit(int c);

/* ---------- Utils (src/utils/second_utils.c) ---------- */
int     is_space(char c);
void    skip_spaces(const char **cursor);
int     is_empty_line(const char *line);

/* ---------- Scene lifecycle (src/utils/parse_utils.c) ---------- */
void    init_scene_outputs(t_scene *scene);
void    free_textures(t_textures *textures);
void    free_map(t_map *map);
void    free_lines(char **lines);  /* FIX: Nouvelle déclaration */
char  **load_lines(const char *path, t_scene *scene);
void    cleanup_scene(t_scene *scene);

/* ---------- High-level parse helpers (src/utils/second_parse_utils.c) ---------- */
int     parse_headers_and_map(char **lines, t_scene *scene, int *map_start);
int     scan_and_set_spawn(t_scene *scene);
int     finish_map(t_scene *scene);

/* file_validtion_utils.c: */
int	reader_fail(t_file_reader *reader, const char *msg);
int	reader_realloc(t_file_reader *reader, size_t new_capacity);
int	reader_ensure_capacity(t_file_reader *reader, size_t need);
void	reader_init(t_file_reader *reader, t_scene *scene);
int	reader_append_buffer(t_file_reader *reader, char *chunk_buf, ssize_t bytes_read);

/* file_split_utils.c: */
void	free_partial(char **lines, size_t n);
size_t	count_total_lines(const char *filebuf);
char	**alloc_lines_array(size_t total_lines, t_scene *scene);
char	*alloc_line(const char *filebuf, size_t start, size_t end);
int	process_line_split(const char *filebuf, char **lines, size_t *indexes, t_scene *scene);

/* map_parse_utils.c: */
char	*dup_line(const char *s);
int	count_map_rows(char **lines, int map_start);
void	free_partial_map(char **lines_map, int count);
void	reset_map_data(t_map *out);
void	update_max_cols(t_map *out, int r);


/* ---------- File I/O (src/file/) ---------- */
int     open_check(const char *path, t_scene *scene);
char   *read_file(int fd, t_scene *scene);
char  **split_lines(const char *filebuf, t_scene *scene);

/* ---------- Headers parsing (src/parse/) ---------- */
int     parse_headers(char **lines, int *out_map_start, t_scene *scene);
int     match_texture_line(const char *line, const char *ident, char **out_path);
int     match_color_line(const char *line, char id, t_color *out);
int     match_and_set_texture(const char *line, const char *id, char **slot, int *seen_flag);
int     match_and_set_color(const char *line, char id, t_color *slot, int *seen_flag);

/* ---------- Map parsing (src/parse/) ---------- */
int     parse_map_block(char **lines, int map_start, t_map *out, t_scene *scene);
int     make_map_rectangular(t_map *map);
int     check_map_closed(t_map *map);
int     scan_map_for_spawn(t_scene *scene);

/* ---------- Textures validation (src/parse/parse_validate_textures.c) ---------- */
int     validate_textures_exist(t_textures *tx);

/* ---------- Orchestrator & errors (src/parse/) ---------- */
int           parse_scene(const char *path, t_scene *scene);
void          set_parse_error(t_scene *scene, const char *msg);
const char   *get_parse_error(const t_scene *scene);

/* ---------- Inittialisation / init_mlx ---------- */
int	init_mlx(t_game *game, char *title);
void	put_pixel(t_game *game, int x, int y, int color);
void	init_spawn_dir_and_pos(t_player *player, t_spawn *spawn);
int	load_all_textures(t_game *game);
/* ---------- Inittialisation / init_game ---------- */
int	init_game(t_game *game);
void    cleanup_mlx(t_game *game);

void	draw_floor_ceiling(t_game *game);
void	raycasting(t_game *game);
#endif
