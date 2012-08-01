#ifndef PLAYER_H
#define PLAYER_H

typedef struct player
{
  double x, y, xv, yv;
  int jump, air, attack;

  Uint32 color;
} player_t;

player_t * new_player(double x, double y, Uint32 color);
void update_player(player_t * player, platform_t * platform, Uint8 * kestate, SDL_Event * event);
void draw_player(player_t * player, SDL_Surface * screen);
void free_player(player_t * player);

#endif
