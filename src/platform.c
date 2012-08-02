#include <SDL/SDL.h>
#include <stdlib.h>

#include "platform.h"

item_t new_item(item_id_t id, double x, double y, Uint32 color)
{
  item_t item;

  item.x = x;
  item.y = y;
  item.id = id;

  item.xv = item.yv = 0;
  item.color = color;
  
  return item;
}

void update_item(item_t * item)
{
  item->x += item->xv;
  item->y += item->yv;

  item->xv = item->xv / 1.01;

  item->yv -= .005;
  if(item->y < 0)
    {
      item->y = 0;
      item->yv = -item->yv / 1.5;
      if(item->yv < .1 && item->yv > -.1)
	item->yv = 0;
    }
}

void draw_item(item_t * item, SDL_Surface * destination)
{
  SDL_Rect r = {item->x - 8, destination->h - item->y - 16, 16, 16};

  SDL_FillRect(destination, &r, item->color);
}

int find_intersection_item(item_t item, SDL_Rect r)
{
  if(item.x + 8 < r.x || item.x - 8 > r.x + r.w ||
       item.y < r.y || item.y - 16 > r.y + r.h)
    return 0;
  return 1;
}

void add_item(platform_t * platform, item_t item)
{
  int i;
  item_t * items = platform->items;
  
  for(i = 0; i < platform->itemc; i++)
    {
      if(platform->items[i].id == 0)
	{
	  platform->items[i] = item;
	  break;
	}
    }
}

void remove_item(platform_t * platform, int id)
{
  platform->items[id].id = 0;
}

platform_t * new_platform(int itemc)
{
  platform_t * platform = malloc(sizeof(platform_t));

  platform->itemc = itemc;
  platform->items = malloc(sizeof(item_t) * itemc);

  memset(platform->items, 0, sizeof(item_t) * itemc);
  
  return platform;
}

void update_platform(platform_t * platform)
{
  int i;

  for(i = 0; i < platform->itemc; i++)
    {
      if(platform->items[i].id != ID_NULL)
	{
	  update_item(&(platform->items[i]));
	}
    }
}

void draw_platform(platform_t * platform, SDL_Surface * destination)
{
  int i;

  for(i = 0; i < platform->itemc; i++)
    {
      if(platform->items[i].id != ID_NULL)
	{
	  draw_item(&(platform->items[i]), destination);
	}
    }
}
  
void free_platform(platform_t * platform)
{
  free(platform->items);
  free(platform);
}
