#ifndef RATIUM_H
#define RATIUM_H

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ever (;;)

#define RED COLOR_PAIR(1)
#define GREEN COLOR_PAIR(2)
#define BLUE COLOR_PAIR(3)
#define YELLOW COLOR_PAIR(4)
#define BROWN COLOR_PAIR(5)
#define GREY COLOR_PAIR(6)
#define DARK_YELLOW COLOR_PAIR(8)
#define WATER COLOR_PAIR(9)
#define GRASS COLOR_PAIR(10)

#define MAX_X 80
#define MAX_Y 24
#define MAX_NAME 16
#define MAX_PLAYERS 8
#define MAX_ENTITIES 256
#define MAX_ITEMS 256
#define MAX_INV_SLOTS 16

typedef enum {
	LEFT,
	DOWN,
	UP,
	RIGHT
} DIREC;

typedef enum {
	ITEM_MISC,
	ITEM_FOOD,
	ITEM_SWORD,
	ITEM_SHIELD
} ITEM_TYPE;

typedef enum {
	ENT_PLAYER,
	ENT_HOSTILE,
	ENT_PEACEFUL
} ENT_TYPE;

/* TODO: Make inv point to a item */
typedef struct {
	char *name;
	ITEM_TYPE type;
	char face;
	int color;
	int qty;

	int stat;
} inv_t;

typedef struct {
	char *name;
	ITEM_TYPE type;
	char map[MAX_Y][MAX_X];
	char face;
	int color;

	int stat;
} item_t;

/* TODO: add sight range */
typedef struct {
	char *name;
	char *drop;
	ENT_TYPE type;
	char face;
	int color;

	DIREC direc;
	int x, y;
	int bary;

	int maxhp, hp;
	bool isdead;
	item_t holding;
	int damage;

	inv_t inv[MAX_INV_SLOTS];
} entity_t;

/* TODO: localize some of these functions */
/***********************\
* map.c: handle the map *
\***********************/
char get_map(int x, int y);
void set_map(int x, int y, char newch);
bool is_floor(int x, int y);
int floor_count(char ch);
void toggle_door(int x, int y);
void draw_map(entity_t e, int r);
void draw_map_floor(entity_t e, int r);

/**********************\
* item.c: handle items *
\**********************/
int  query_item(char *name);
void clear_item(item_t *item, int x, int y);
void add_item(item_t *item, int x, int y);
void add_item_ch(item_t *item, int x, int y, char newch);
void draw_item(item_t item, entity_t e, int r);

/*****************************************\
* entity.c: handle anything with entities *
\*****************************************/
bool can_step(entity_t *e, int x, int y);
void move_entity(entity_t *e, int x_0, int y_0);
void attack(entity_t *e, entity_t *foe);
int  deal_damage(entity_t *e);
void take_damage(entity_t *e, int damge);
bool isalive(int hp);
int  holding_x(entity_t e, int val);
int  holding_y(entity_t e, int val);
void draw_ent(entity_t e, entity_t oe, int r);
void rand_ai(entity_t *e, int speed);
void dumb_ai(entity_t *e, int xNew, int yNew, int speed);

/*****************************\
* player.c: handle the player *
\*****************************/
void get_item(entity_t *e);
void player_run(int c, entity_t *e);

/**************************************\
* inv.c: handle the player's inventory *
\**************************************/
void inv(entity_t *e);
void draw_inv(entity_t *e, int arrow_y);
void inv_add_item(entity_t *e, item_t *item, int qty);
void inv_use_item(entity_t *e, int num);
void inv_drop_item(entity_t *e, int num);

/*********************************************\
* data.c: handle reading from data/ directory *
\*********************************************/
void init_item(int to, int from);
void init_entity(void);
void init_player(int from, int to);

int itemqty;
int playerqty;
int entqty;

item_t item[MAX_ITEMS];
entity_t player[MAX_PLAYERS];
entity_t entity[MAX_ENTITIES];

#endif
