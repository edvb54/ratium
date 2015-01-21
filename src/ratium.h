#ifndef RATIUM_H
#define RATIUM_H

#include <ncurses.h>
#include <stdio.h>

#define MAX_ENTITIES 256

#define RED COLOR_PAIR(1)
#define GREEN COLOR_PAIR(2)
#define BLUE COLOR_PAIR(3)
#define YELLOW COLOR_PAIR(4)
#define BROWN COLOR_PAIR(5)
#define GREY COLOR_PAIR(6)
#define DARK_YELLOW COLOR_PAIR(8)
#define WATER COLOR_PAIR(9)
#define GRASS COLOR_PAIR(10)

typedef struct {
	char map[24][81];
	char face;
	int color;
} item_t;

typedef struct {
	int x, y;
	char face;
	int color;

	int gold;
	int hp, hpFull;
	int damage;
	char holding;
} entity_t;

/* map.c */
void draw_map();

/* item.c */
void init_item(item_t *item);
void clear_item(item_t *item, int x, int y);
void draw_item(item_t item);

/* door.c */
void toggle_door(int x, int y);

/* entity.c */
bool can_step(int x, int y);
void move_entity(entity_t *e, int x_0, int y_0);

/* player.c */
void get_item(entity_t *e);
void player_run(char c, entity_t *e);

item_t gold;
item_t door;
entity_t entity[MAX_ENTITIES];

#endif
