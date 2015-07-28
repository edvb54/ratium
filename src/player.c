#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ratium.h"
#include "ent.h"
#include "gfx.h"

void add_msg(Msg *msg, char *message) {
	strcpy(msg->data, message);
	msg->disp = true;
}

void draw_msg(Msg *msg) {
	if (msg->disp == true) {
		int x, y;
		x = (maxx / 2) - (strlen(msg->data) / 2);
		y = maxy / 2;
		rat_mvprint(x, y, msg->data, 12);
		msg->disp = false;
	}
}

static void draw_inv(Ent *e, int arrow_y) {
	char s[50];

	rat_clear();

	rat_mvprint(0, 0, " -- Inventory -- \n", -1);

	for (int i = 0; i < MAX_INV; i++)
		if (e->inv[i].face != ' ') {
			sprintf(s, "   %c)", i + 97);
			rat_print(s, 6);

			sprintf(s, " %c", e->inv[i].face);
			rat_print(s, e->inv[i].color);

			sprintf(s, " %s", e->inv[i].name);
			rat_print(s, 0);

			sprintf(s, " (%d)\n", e->inv[i].map[0][0]);
			rat_print(s, 6);
		}

	rat_mvprint(1, arrow_y, ">", 0);

}

static void inv_add_item(Ent *e, Item *item, int qty) {
	for (int i = 0; i <= MAX_INV; i++)
		if (e->inv[i].face == ' ') {
			e->inv[i].face = item->face;
			e->inv[i].name = item->name;
			e->inv[i].color = item->color;
			e->inv[i].type = item->type;
			e->inv[i].stat = item->stat;
			e->inv[i].map[0][0] = qty;
			return;
		} else if (e->inv[i].name == item->name) {
			e->inv[i].map[0][0] += qty;
			return;
		}
}

static void inv_use_item(Ent *e, int num) {
	if (e->inv[num].map[0][0] > 0)
		switch (e->inv[num].type) {
			case ITEM_MISC:
				break;
			case ITEM_FOOD:
				e->hp += e->inv[num].stat;
				e->inv[num].map[0][0]--;
				break;
			case ITEM_SWORD:
			case ITEM_SHIELD:
				if (e->holding.face == ' ') {
					e->holding.name  = e->inv[num].name;
					e->holding.face  = e->inv[num].face;
					e->holding.color = e->inv[num].color;
					e->holding.type  = e->inv[num].type;
					e->holding.stat  = e->inv[num].stat;
					e->inv[num].map[0][0]--;
				}
				break;
		}
}

static void inv_drop_item(Ent *e, int num) {
	if (e->inv[num].map[0][0] > 0) {
		for (int i = 0; i < MAX_ITEMS; i++)
			if (e->inv[num].face == item[i].face)
				if (e->inv[num].color == item[i].color)
					add_item(&item[i], e->x, e->y);
		e->inv[num].map[0][0]--;
	}
}

static void inv(Ent *e) {
	int arrow_y = 1;
	int k;

	do {
		if      (k == e->keys.up)   { arrow_y--; }
		else if (k == e->keys.down) { arrow_y++; }
		else if (k == e->keys.get)  { inv_use_item(e, arrow_y-1); }
		else if (k == e->keys.drop) { inv_drop_item(e, arrow_y-1); }

		if (e->inv[arrow_y-1].face == ' ')
			arrow_y--;

		/* TODO: Make moving arrow work better so this is not needed */
		if (arrow_y <= 0)
			arrow_y = 1;

		draw_inv(e, arrow_y);

	} while ((k = rat_getch()) != e->keys.inv);

	rat_clear();

}

static void get_item(Ent *e) {
	for (int i = 0; i <= itemqty; i++)
		if (item[i].map[e->y][e->x] > 0) {
			inv_add_item(e, &item[i], 1);
			clear_item(&item[i], e->x, e->y);
			return;
		}
	/* TODO: Break into function */
	/* if nothing is under player put what player was holding into inv */
	if (e->holding.face != ' ') {
		e->holding.face = ' ';
		e->holding.color = 0;
		e->holding.type = 0;
		e->holding.stat = 0;
		for (int i = 0; i < MAX_INV; i++)
			if (strcmp(e->inv[i].name, e->holding.name) == 0)
				e->inv[i].map[0][0]++;
	}
}

bool player_run(int c, Ent *e) {
	if (isalive(e->hp)) {
		bool returnval = true; /* true if key pressed is valid key */

		if (c == e->keys.left) {
			move_entity(e, -1, 0);
			e->direc = LEFT;
		} else if (c == e->keys.down) {
			move_entity(e, 0, 1);
			e->direc = DOWN;
		} else if (c == e->keys.up) {
			move_entity(e, 0, -1);
			e->direc = UP;
		} else if  (c == e->keys.right) {
			move_entity(e, 1, 0);
			e->direc = RIGHT;
		} else if  (c == e->keys.leftdown) {
			move_entity(e, -1, 1);
			e->direc = LEFTDOWN;
		} else if  (c == e->keys.leftup) {
			move_entity(e, -1, -1);
			e->direc = LEFTUP;
		} else if (c == e->keys.rightdown) {
			move_entity(e, 1, 1);
			e->direc = RIGHTDOWN;
		} else if (c == e->keys.rightup) {
			move_entity(e, 1, -1);
			e->direc = RIGHTUP;
		}
		else if (c == e->keys.stand) { returnval = true; }
		else if (c == e->keys.get)   { get_item(e); }
		else if (c == e->keys.open)  { toggle_door(e->x, e->y); }
		else if (c == e->keys.inv)   { inv(e); returnval = true; } /* TODO: Make inv not take up turn */
		else { returnval = false; }

		if (e->hp > e->maxhp)
			e->hp = e->maxhp;

		return returnval;

	} else if (!e->isdead) {
		add_msg(&player[0].msg, "You Died!");
		e->isdead = true;
		return true;
	} else
		return false;
}

