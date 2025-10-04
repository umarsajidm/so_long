#include "so_long.h"
#include <stdio.h>

int	close_game(t_game *g)
{
	free_map(g->map);
	mlx_destroy_window(g->mlx,g->win);
	exit(0);
}

int	handle_key(int keycode, t_game *g)
{
	int nx=g->player_x, ny=g->player_y;
	if(keycode==65307) close_game(g);
	if(keycode==119||keycode==65362) ny--; // W / Up
	if(keycode==115||keycode==65364) ny++; // S / Down
	if(keycode==97 ||keycode==65361) nx--; // A / Left
	if(keycode==100||keycode==65363) nx++; // D / Right
	if(g->map[ny][nx]!='1'){
		if(g->map[ny][nx]=='C') g->collectibles--;
		if(g->map[ny][nx]=='E'&&g->collectibles==0){
			printf("You win in %d moves!\n",++g->moves);
			close_game(g);
		}
		g->map[g->player_y][g->player_x]='0';
		g->player_x=nx; g->player_y=ny;
		g->map[ny][nx]='P';
		printf("Moves: %d\n",++g->moves);
		render_map(g);
	}
	return 0;
}
