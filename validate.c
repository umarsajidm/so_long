#include "so_long.h"
#include <stdio.h>

static void	check_walls(t_game *g)
{
	int x, y;

	for (x = 0; x < g->width; x++)
		if (g->map[0][x] != '1' || g->map[g->height-1][x] != '1')
			exit(write(2, "Error\nMap not closed\n", 21));
	for (y = 0; y < g->height; y++)
		if (g->map[y][0] != '1' || g->map[y][g->width-1] != '1')
			exit(write(2, "Error\nMap not closed\n", 21));
}

static void flood_fill(char **map, int x, int y, t_game *g)
{
	if (x<0||y<0||x>=g->width||y>=g->height) return;
	if (map[y][x]=='1'||map[y][x]=='F') return;
	map[y][x] = 'F';
	flood_fill(map,x+1,y,g);
	flood_fill(map,x-1,y,g);
	flood_fill(map,x,y+1,g);
	flood_fill(map,x,y-1,g);
}

static char	**dup_map(t_game *g)
{
	int i; char **dup = malloc(sizeof(char*)*(g->height+1));
	for (i=0;i<g->height;i++) dup[i]=ft_strdup(g->map[i]);
	dup[i]=NULL; return dup;
}

void	validate_map(t_game *g)
{
	int p=0,e=0,c=0,x,y; char **test;
	check_walls(g);
	for (y=0;y<g->height;y++) for (x=0;x<g->width;x++)
	{
		if (g->map[y][x]=='P'){g->player_x=x;g->player_y=y;p++;}
		else if (g->map[y][x]=='E') e++;
		else if (g->map[y][x]=='C') c++;
		else if (g->map[y][x]!='0'&&g->map[y][x]!='1')
			exit(write(2,"Error\nInvalid char\n",19));
	}
	if (p!=1||e!=1||c<1) exit(write(2,"Error\nNeed 1P 1E >=1C\n",22));
	g->collectibles=c;
	test=dup_map(g);
	flood_fill(test,g->player_x,g->player_y,g);
	int rc=0,re=0;
	for(y=0;y<g->height;y++)for(x=0;x<g->width;x++){
		if(g->map[y][x]=='C'&&test[y][x]=='F')rc++;
		if(g->map[y][x]=='E'&&test[y][x]=='F')re++;
	}
	if(rc!=c||re!=1) exit(write(2,"Error\nNo valid path\n",20));
	free_map(test);
}
