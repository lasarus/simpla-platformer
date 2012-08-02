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
  player->attack = 0;

  player->health = player->maxhealth = 1;

  player->armour = 0;
  
  return player;
}

void damage_player(player_t * player, double health)
{
  player->armour -= health;
  if(player->armour > 0)
    return;
  player->health += player->armour;
  if(player->health < 0)
    player->health = player->maxhealth;
  if(player->armour < 0)
    player->armour = 0;
}

void heal_player(player_t * player, double health)
{
  player->health += health;
  if(player->health > player->maxhealth)
    player->health = player->maxhealth;
}

void update_player(player_t * player, platform_t * platform, Uint8 * keystate, SDL_Event * event)
{
  int i;
  double speed = .5;
  
  player->x += player->xv;
  player->y += player->yv;
  
  player->xv = player->xv / 1.01;
  if(player->xv < 1 && player->attack == 1)
    player->attack = 0;
  
  player->yv -= .005;
  if(player->y < 0)
    {
      if(player->yv < -3)
	damage_player(player, -player->yv / 32);
      
      if(player->attack == 2)
	{
	  player->attack = 0;
	}
      
      player->y = 0;
      player->yv = 0;

      player->air = 0;
      player->jump = 0;
    }

  if(keystate[SDLK_a])
    {
      player->xv = -speed;
      if(keystate[SDLK_LSHIFT])
	{
	  player->xv *= 2;
	  player->attack = 1;
	}
      if(keystate[SDLK_LCTRL])
	{
	  player->xv = .5;
	}
    }
  else if(keystate[SDLK_d])
    {
      player->xv = speed;
      if(keystate[SDLK_LSHIFT])
	{
	  player->xv *= 2;
	  player->attack = 1;
	}
      if(keystate[SDLK_LCTRL])
	{
	  player->xv = .5;
	}
    }

  if(keystate[SDLK_s] && player->air)
    {
      if(player->yv > -4)
	player->yv = -4;
      player->attack = 2;
    }

  if(event != NULL &&
     (*event).type == SDL_KEYDOWN)
    {
      if((*event).key.keysym.sym == SDLK_w && player->jump < 2)
	{
	  player->yv = 1;
	  if(player->jump == 0 && keystate[SDLK_LSHIFT])
	    player->yv = 1.5;
	  player->jump++;
	  player->air = 1;
	}
      else if((*event).key.keysym.sym == SDLK_q)
	{
	  add_item(platform, new_item(ID_HEAL, player->x, player->y + 480, rand() % 0x1000000));
	}
    }

  SDL_Rect cr = {player->x - 8, player->y - 16, 16, 16};
  for(i = 0; i < platform->itemc; i++)
    {
      if(platform->items[i].id != ID_NULL && find_intersection_item(platform->items[i], cr))
	{
	  if(player->attack == 1 || player->attack == 2)
	    {
	      switch(platform->items[i].id)
		{
		case ID_HEAL:
		  heal_player(player, .1);
		  break;
		  
		case ID_SUPERHEAL:
		  heal_player(player, .1);
		  break;
		  
		case ID_INCREASE_HEALTH:
		  player->health /= player->maxhealth;
		  player->health *= player->maxhealth + 1;
		  player->maxhealth += 1;
		  break;
		  
		case ID_ARMOUR:
		  player->armour += 1;
		  break;
		}
	      
	      platform->items[i].id = ID_NULL;
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
  SDL_Rect hbarr = {2, 2, player->armour * 64 + player->health * 64 + 4, 20};
  SDL_FillRect(destination, &hbarr, 0xFFFFFF);
  
  SDL_Rect r = {player->x - 8, destination->h - player->y - 16, 16, 16};
  SDL_FillRect(destination, &r, player->color);

  SDL_Rect healthr = {4, 4, player->health * 64, 16};
  SDL_FillRect(destination, &healthr, 0xFF0000);

  SDL_Rect armourr = {4 + player->health * 64, 4, player->armour * 64, 16};
  SDL_FillRect(destination, &armourr, 0xFF);
}

void free_player(player_t * player)
{
  free(player);
}
