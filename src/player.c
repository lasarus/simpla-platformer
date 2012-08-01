#include <SDL/SDL.h>
#include <math.h>
#include <stdlib.h>

#include "platform.h"
#include "player.h"

player_t * new_player(double x, double y, Uint32 color)
{
  player_t * player = malloc(sizeof(player_t));

  player->x = x;
  player->y = y;

  player->yv = player->xv = 0;
  player->air = player->jump = 1;

  player->color = color;
  
  return player;
}

void update_player(player_t * player, platform_t * platform, Uint8 * keystate, SDL_Event * event)
{
  int i;
  double speed = .5;
  
  player->x += player->xv;
  player->y += player->yv;

  player->xv = player->xv / 1.01;

  player->yv -= .005;
  if(player->y < 0)
    {
      player->y = 0;
      player->yv = 0;

      player->air = 0;
      player->jump = 0;
    }

  if(keystate[SDLK_a])
    player->xv = -speed * (keystate[SDLK_LSHIFT] ? 2 : 1) * (keystate[SDLK_LCTRL] ? .5 : 1);
  else if(keystate[SDLK_d])
    player->xv = speed * (keystate[SDLK_LSHIFT] ? 2 : 1) * (keystate[SDLK_LCTRL] ? .5 : 1);

  if(keystate[SDLK_s] && player->air)
    player->yv = -4;
  
  if(event != NULL &&
     (*event).type == SDL_KEYDOWN)
    {
      if((*event).key.keysym.sym == SDLK_w && player->jump < 3)
	{
	  player->yv = 1;
	  player->jump++;
	  player->air = 1;
	}
      else if((*event).key.keysym.sym == SDLK_q)
	{
	  add_item(platform, new_item(1, player->x, player->y + 480, rand() % 0x1000000));
	}
    }
  
  SDL_Rect cr = {player->x - 8, player->y - 16, 16, 16};
  for(i = 0; i < platform->itemc; i++)
    {
      if(find_intersection_item(platform->items[i], cr))
	{
	  
	  if(fabs(player->xv) > .75 || fabs(player->yv) > .75)
	    {
	      platform->items[i].id = 0;
	    }
	  
	  else
	    {
	      platform->items[i].xv = player->xv * 1.1;
	    }
	}
    }
}

void draw_player(player_t * player, SDL_Surface * destination)
{
  SDL_Rect r = {player->x - 8, destination->h - player->y - 16, 16, 16};

  SDL_FillRect(destination, &r, player->color);
}

void free_player(player_t * player)
{
  free(player);
}
