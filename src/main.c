#include <SDL/SDL.h>
#include <stdlib.h>
#include <time.h>

#include "platform.h"
#include "player.h"

int s_w = 640, s_h = 480, s_bpp = 32;
int quit = 0;

SDL_Surface * screen = NULL;
SDL_Event event;

int init()
{
  if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
    return 1;

  screen = SDL_SetVideoMode(s_w, s_h, s_bpp, SDL_SWSURFACE);

  if(screen == NULL)
    return 1;

  return 0;
}

int main(int arg, char ** argv)
{
  player_t * player;
  platform_t * platform;
  Uint8 * keystate;
  int nev;

  srand(time(NULL));
  
  if(init())
    return 1;

  keystate = NULL;
  player = new_player(s_w / 2, s_h, 0xFFFFFF);
  platform = new_platform(100);

  add_item(platform, new_item(ID_HEAL, s_w / 4, s_h - 20, 0xFF0000));
  //add_item(platform, new_item(ID_SUPERHEAL, (s_w * 2) / 4, s_h - 20, 0xFF0000));
  add_item(platform, new_item(ID_ARMOUR, (s_w * 3) / 4, s_h - 20, 0xFF));
  
  while(!quit)
    {
      if(SDL_PollEvent(&event))
	{
	  nev = 1;
	  if(event.type == SDL_QUIT)
	    quit = 1;
	}
      else
	nev = 0;
      /*Update Loop*/
      keystate = SDL_GetKeyState(NULL);

      update_player(player, platform, keystate, (nev) ? &event : NULL);
      update_platform(platform);
      
      /*Draw Loop*/
      SDL_FillRect(screen, NULL, 0);

      draw_player(player, screen);
      draw_platform(platform, screen);
      
      if(SDL_Flip(screen))
	return 1;
    }
  free_player(player);
  return 0;
}
