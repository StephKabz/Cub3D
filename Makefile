NAME    := cub3d
CC      := cc
CFLAGS  := -Wall -Wextra -Werror -g
INCS    := -Iinclude -Iminilibx-linux
MLX_DIR := minilibx-linux
MLX_LIB := $(MLX_DIR)/libmlx.a
MLX_FLAGS := -L$(MLX_DIR) -lmlx -lXext -lX11 -lm

OBJ_DIR := obj

SRCS    := src/file/file_validation.c \
           src/file/read_file.c \
           src/file/split_lines.c \
           src/parse/parse.c \
           src/parse/parse_headers.c \
           src/parse/parse_headers_assign.c \
           src/parse/parse_headers_colors.c \
           src/parse/parse_headers_textures.c \
           src/parse/parse_validate_textures.c \
           src/parse/parse_map_block.c \
           src/parse/parse_spawn.c \
           src/parse/parse_map_rectangle.c \
           src/parse/parse_check_map_closed.c \
           src/parse/parse_error.c \
           src/utils/second_utils.c \
           src/utils/utils.c \
           src/utils/parse_utils.c \
           src/utils/second_parse_utils.c \
           src/utils/file_validation_utils.c \
           src/utils/file_split_utils.c \
           src/utils/map_parse_utils.c \
           src/execution/init_mlx.c \
           src/execution/init_game.c \
           src/execution/draw_floor_ceiling.c \
           src/execution/raycasting_utils.c \
           src/execution/raycasting_utils1.c \
           src/execution/raycasting_utils2.c \
           src/execution/raycasting.c \
           src/execution/moves.c \
           src/execution/inputs_and_move.c \
           src/execution/game_loop.c \
           src/main.c

# Utiliser patsubst pour la transformation
OBJS := $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(SRCS))

all: $(MLX_LIB) $(NAME)

$(MLX_LIB):
	@echo "Compiling MLX..."
	@make -C $(MLX_DIR) 2>/dev/null || true

$(NAME): $(OBJS) $(MLX_LIB)
	@echo "Linking $(NAME)..."
	$(CC) $(CFLAGS) $(OBJS) $(MLX_FLAGS) -o $(NAME)
	@echo "✅ $(NAME) created!"

$(OBJ_DIR)/%.o: src/%.c include/cubd3.h
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

clean:
	@echo "Cleaning objects..."
	@rm -rf $(OBJ_DIR)
	@make -C $(MLX_DIR) clean 2>/dev/null || true
	@echo "✅ Objects cleaned!"

fclean: clean
	@echo "Cleaning $(NAME)..."
	@rm -f $(NAME)
	@echo "✅ Everything cleaned!"

re: fclean all

debug:
	@echo "SRCS:"
	@echo "$(SRCS)" | tr ' ' '\n'
	@echo ""
	@echo "OBJS:"
	@echo "$(OBJS)" | tr ' ' '\n'

.PHONY: all clean fclean re debug
