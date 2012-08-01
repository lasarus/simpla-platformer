#ifndef PLATFORM_H
#define PLATFORM_H

typedef struct item
{
  int id;
  Uint32 color;

  double x, y, xv, yv;
} item_t;

typedef struct platform
{
  item_t * items;
  int itemc;
} platform_t;

item_t new_item(int id, double x, double y, Uint32 color);
void update_item(item_t * item);
void draw_item(item_t * item, SDL_Surface * destination);
int find_intersection_item(item_t item, SDL_Rect r);

void add_item(platform_t * platform, item_t item);
void remove_item(platform_t * platform, int id);

platform_t * new_platform(int itemc);
void update_platform(platform_t * platform);
void draw_platform(platform_t * platform, SDL_Surface * destination);
void free_platform(platform_t * platform);

#endif
