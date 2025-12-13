/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cubd3.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkabang <stkabang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 16:28:31 by stkabang          #+#    #+#             */
/*   Updated: 2025/12/13 19:19:54 by stkabang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include <math.h>
# include <mlx.h>
# include <mlx_int.h>

# define READ_BUFFER 4096
# define WIDTH 1500
# define HEIGHT 1500
# define MOVE_SPEED 0.2
# define ROT_SPEED 0.03
# define KEY_W 119
# define KEY_A 97
# define KEY_S 115 
# define KEY_D 100
# define KEY_LEFT 65361
# define KEY_RIGHT 65363
# define KEY_ESC 65307

/* ---------- Types_parsing ---------- */

typedef struct s_textures
{
	char	*no;
	char	*so;
	char	*we;
	char	*ea;
}	t_textures;

typedef struct s_color
{
	int	red;
	int	green;
	int	blue;
	int	defined;
}	t_color;

typedef struct s_fc_colors
{
	t_color	floor;
	t_color	ceiling;
}	t_fc_colors;

typedef struct s_seen
{
	int	no;
	int	so;
	int	we;
	int	ea;
	int	f;
	int	c;
}	t_seen;

typedef struct s_map
{
	char	**lines_map;
	int		rows;
	int		cols;
}	t_map;

typedef struct s_spawn
{
	int		found;
	int		row;
	int		col;
	char	dir;
}	t_spawn;

typedef struct s_scene
{
	t_textures	textures;
	t_fc_colors	fc;
	t_map		map;
	t_spawn		spawn;
	char		*err;
}	t_scene;

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
	float	pos_x;
	float	pos_y;
	float	dir_x;
	float	dir_y;
	float	plane_x;
	float	plane_y;
}	t_player;

typedef struct s_textures_img
{
	void	*img;
	char	*img_data;
	int		width;
	int		height;
	int		bpp;
	int		line_len;
	int		endian;
}	t_textures_img;

typedef struct s_all_textures
{
	t_textures_img	north;
	t_textures_img	south;
	t_textures_img	west;
	t_textures_img	east;
}	t_all_textures;

typedef struct s_ray
{
	double	ray_dir_x;
	double	ray_dir_y;
	int		map_x;
	int		map_y;
	double	side_dist_x;
	double	side_dist_y;
	double	delta_dist_x;
	double	delta_dist_y;
	int		step_x;
	int		step_y;
	int		hit;
	int		side;
	double	perp_dist_wall;
}	t_ray;

typedef struct s_keys
{
	int	w;
	int	s;
	int	a;
	int	d;
	int	left;
	int	right;
	int	esc;
}	t_keys;

typedef struct s_tex_params
{
	int		tex_x;
	double	step;
	double	tex_pos;
}	t_tex_params;

typedef struct s_render_params
{
	int		draw_start;
	int		draw_end;
	int		floor_color;
	int		ceiling_color;
	t_textures_img	*texture;
	t_tex_params	tex;
}	t_render_params;

typedef struct s_game
{
	void			*mlx;
	void			*win;
	void			*img;
	char			*img_data;
	int				bpp;
	int				line_len;
	int				endian;
	t_scene			scene;
	t_all_textures	textures;
	t_player		player;
	t_keys			keys;
}	t_game;

/* ---------- Utils (src/utils/) ---------- */
char		*ft_strrchr(const char *s, int c);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
size_t		ft_strlen(const char *s);
void		*ft_memcpy(void *dst, const void *src, size_t n);
int			ft_isdigit(int c);
/* ---------- Utils (src/utils/second_utils.c) ---------- */
int			is_space(char c);
void		skip_spaces(const char **cursor);
int			is_empty_line(const char *line);
/* ---------- Scene lifecycle (src/utils/parse_utils.c) ---------- */
void		init_scene_outputs(t_scene *scene);
void		free_textures(t_textures *textures);
void		free_map(t_map *map);
void		free_lines(char **lines);/* FIX: Nouvelle déclaration */
char		**load_lines(const char *path, t_scene *scene);
void		cleanup_scene(t_scene *scene);
/* ---------- High-level parse helpers ---------- */
int			parse_headers_and_map(char **lines, t_scene *scene, int *map_start);
int			scan_and_set_spawn(t_scene *scene);
int			finish_map(t_scene *scene);
/* file_validtion_utils.c: */
int			reader_fail(t_file_reader *reader, const char *msg);
int			reader_realloc(t_file_reader *reader, size_t new_capacity);
int			reader_ensure_capacity(t_file_reader *reader, size_t need);
void		reader_init(t_file_reader *reader, t_scene *scene);
int			reader_append_buffer(t_file_reader *reader, char *chunk_buf, ssize_t bytes_read);

/* file_split_utils.c: */
void		free_partial(char **lines, size_t n);
size_t		count_total_lines(const char *filebuf);
char		**alloc_lines_array(size_t total_lines, t_scene *scene);
char		*alloc_line(const char *filebuf, size_t start, size_t end);
int			process_line_split(const char *filebuf, char **lines, size_t *indexes, t_scene *scene);

/* map_parse_utils.c: */
char		*dup_line(const char *s);
int			count_map_rows(char **lines, int map_start);
void		free_partial_map(char **lines_map, int count);
void		reset_map_data(t_map *out);
void		update_max_cols(t_map *out, int r);
/* ---------- File I/O (src/file/) ---------- */
int			open_check(const char *path, t_scene *scene);
char		*read_file(int fd, t_scene *scene);
char		**split_lines(const char *filebuf, t_scene *scene);

/* ---------- Headers parsing (src/parse/) ---------- */
int			parse_headers(char **lines, int *out_map_start, t_scene *scene);
int			match_texture_line(const char *line, const char *ident, char **out_path);
int			match_color_line(const char *line, char id, t_color *out);
int			match_and_set_texture(const char *line, const char *id, char **slot, int *seen_flag);
int			match_and_set_color(const char *line, char id, t_color *slot, int *seen_flag);

/* ---------- Map parsing (src/parse/) ---------- */
int			parse_map_block(char **lines, int map_start, t_map *out, t_scene *scene);
int			make_map_rectangular(t_map *map);
int			check_map_closed(t_map *map);
int			scan_map_for_spawn(t_scene *scene);

/* ---------- Textures validation ---------- */
int			validate_textures_exist(t_textures *tx);
/* ---------- Orchestrator & errors (src/parse/) ---------- */
int			parse_scene(const char *path, t_scene *scene);
void		set_parse_error(t_scene *scene, const char *msg);
const char	*get_parse_error(const t_scene *scene);

/* ---------- Inittialisation / init_mlx ---------- */
int			init_mlx(t_game *game, char *title);
void		put_pixel(t_game *game, int x, int y, int color);
void		init_spawn_dir_and_pos(t_player *player, t_spawn *spawn);
int			load_all_textures(t_game *game);
/* ---------- Inittialisation / init_game ---------- */
int			init_game(t_game *game);
void		cleanup_mlx(t_game *game);

void		draw_floor_ceiling(t_game *game);
void		raycasting(t_game *game);

/* ---------- movements and rotation ---------- */
int			key_press(int keycode, void *param);
int			key_release(int keycode, void *param);
void		handle_movement(t_game *game);
void		handle_rotation(t_game *game);
/* ---------- game loop ---------- */
int			game_loop(void *param);
int			close_window(void *param);

/* ---------- moves_functions ---------- */
void		check_wall(t_game *game, float newX, float newY);
void		move_forward(t_game *game);
void		move_backward(t_game *game);
void		move_right(t_game *game);
void		move_left(t_game *game);
/* ---------- raycasting_utils ---------- */
double		ft_floor(double nb);
double		ft_abs_double(double nb);
/* ---------- raycasting_utils1 ---------- */
void		init_ray(t_ray *ray, t_player *player, int x);
/* ---------- raycasting_utils2 ---------- */
void		do_DDA(t_ray *ray, t_map *map);
void		calculate_perp_dist(t_ray * ray, t_player *player);
void		floor_ceiling_color(t_game *game, int *floor, int *ceiling);
void		wall_bounds(t_ray *ray, int *draw_start, int *draw_end, double *line_height);
double		calculate_wall_x(t_game *game, t_ray *ray);
/* ---------- raycasting ---------- */
void		raycasting(t_game *game);
#endif
