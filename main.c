#include "so_long.h"

int	main(int argc, char **argv)
{
	t_game	game;

	if (argc != 2)
		return (write(2, "Usage: ./so_long map.ber\n", 25), 1);
	game.moves = 0;
	game.map = load_map(argv[1], &game);
	validate_map(&game);

	game.mlx = mlx_init();
	game.win = mlx_new_window(game.mlx, game.width * 32,
			game.height * 32, "so_long");

	render_map(&game);
	mlx_key_hook(game.win, handle_key, &game);
	mlx_hook(game.win, 17, 0, close_game, &game);
	mlx_loop(game.mlx);
	return (0);
}
