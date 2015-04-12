#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "ratium.h"

/* TODO: Improve and implement in other functions */
/* TODO: Add min and max parameters */
/* calc_rarity: change rarity to make it random */
static void calc_rarity(int *rarity) {
	if (*rarity != 0)
		*rarity += rand() % 3;
	if (*rarity + entqty > MAX_ENTITIES)
		*rarity = 0;
}

/* us_to_space: convert underscores in string name to spaces */
static void us_to_space(char *data) {
	for(int i = 0, l = strlen(data); i < l; i++)
		if(data[i] == '_') {
			data[i] = ' ';
			continue;
		}
}

/* init_item: read from data/items.txt file and store in item array */
void init_item(void) {

	char *name = malloc(MAX_NAME * sizeof(char));
	char face;
	int color;
	int type;
	int stat;
	int rarity;
	itemqty = 0;

	FILE *f = fopen("data/items.txt", "r");

	do {
		fscanf(f, "%s %c(%i): type=%i stat=%i rarity=%i\n",
			   name, &face, &color, &type, &stat, &rarity);

		us_to_space(name);

		item[itemqty].name = malloc(MAX_NAME * sizeof(char));
		strcpy(item[itemqty].name, name);
		item[itemqty].face = face;
		item[itemqty].color = COLOR_PAIR(color);
		item[itemqty].type = type;
		item[itemqty].stat = stat;

		for (int i = 0; i < MAX_X; i++)
			for (int j = 0; j < MAX_Y; j++)
				item[itemqty].map[j][i] = 0;

		calc_rarity(&rarity);
		for (int x, y, i = 0; i < rarity; i++) {
			do {
				x = rand() % MAX_X;
				y = rand() % MAX_Y;
			} while (get_map(x, y) != '.');
			item[itemqty].map[y][x]++;
		}

		itemqty++;

	} while (!feof(f));

	fclose(f);

	free(name);

	itemqty++;

}

/* init_entity: read from data/entities.txt file and store in entity array */
void init_entity(void) {

	char *name = malloc(MAX_NAME * sizeof(char));
	char *drop = malloc(MAX_NAME * sizeof(char));
	int type;
	int ai;
	char face;
	int color;
	int maxhp;
	int damage;
	int sight;
	int rarity;

	int x_0, y_0;
	int spawntile;

	entqty = 0;

	FILE *f = fopen("data/entities.txt", "r");

	do {
		fscanf(f, "%s %c(%i): hp=%i damge=%i type=%i ai=%i sight=%i rarity=%i %s\n",
			   name, &face, &color, &maxhp, &damage, &type, &ai, &sight, &rarity, drop);

		us_to_space(name);
		us_to_space(drop);

		calc_rarity(&rarity);
		for (int num = 0; num < rarity; num++, entqty++) {
			entity[num].name = malloc(MAX_NAME * sizeof(char));
			strcpy(entity[num].name, name);
			entity[num].type = type;
			entity[num].ai = ai;
			entity[num].face = face;
			entity[num].color = COLOR_PAIR(color);
			entity[num].maxhp = maxhp;
			entity[num].hp = maxhp;
			entity[num].isdead = false;
			entity[num].damage = damage;
			entity[num].sight = sight;

			entity[num].speed = 3;

			entity[num].holding.name = malloc(MAX_NAME * sizeof(char));
			entity[num].holding.face = ' ';
			entity[num].holding.color = 0;
			entity[num].holding.type = 0;
			entity[num].holding.stat = 0;

			entity[num].msg.data = malloc(MAX_NAME * sizeof(char));
			entity[num].msg.disp = false;
			/* TODO: Make this not suck */
			if (strcmp(name, "spock") == 0) {
				strcpy(entity[num].msg.data, "live long and proposer");
				entity[num].msg.disp = true;
			}

			for (int i = 0; i < MAX_INV; i++) {
				entity[num].inv[i].name = malloc(MAX_NAME * sizeof(char));
				entity[num].inv[i].face = ' ';
				entity[num].inv[i].color = 0;
				entity[num].inv[i].map[0][0] = 0;
			}

			strcpy(entity[num].inv[0].name, drop);
			entity[num].inv[0].map[0][0] = rand() % 3;

			switch(type) {
			case TYPE_CAVE:  spawntile = '.'; break;
			case TYPE_GRASS: spawntile = 'g'; break;
			case TYPE_WATER: spawntile = 'w'; break;
			default: spawntile = '.'; break;
			}

			/* TODO: Break into function and add smart
			* integration of is_floor function */
			/* TODO: integrate TYPE_ALL */
			do {
				x_0 = rand() % MAX_X;
				y_0 = rand() % MAX_Y;
			} while (get_map(x_0, y_0) != spawntile);
			entity[num].x = x_0;
			entity[num].y = y_0;

		}
	} while (!feof(f));

	fclose(f);

	free(name);
	free(drop);

}

/* init_player: read from data/players.txt file and store in player array */
void init_player(void) {

	char *name = malloc(MAX_NAME * sizeof(char));
	char face;
	int color;
	int maxhp;
	int damage;
	int sight;

	int x_0, y_0;
	int num = 0;
	int ch, nol = 0;

	FILE *f = fopen("data/players.txt", "r");

	do {
		fscanf(f, "%s %c(%d): hp=%d damage=%d sight=%d",
		       name, &face, &color, &maxhp, &damage, &sight);

		us_to_space(name);

		player[num].name = malloc(MAX_NAME * sizeof(char));
		strcpy(player[num].name, name);
		player[num].type = TYPE_ALL;
		player[num].ai = AI_PLAYER;
		player[num].face = face;
		player[num].color = COLOR_PAIR(color);

		player[num].maxhp = maxhp;
		player[num].hp = maxhp;
		player[num].isdead = false;
		player[num].damage = damage;
		player[num].sight = sight;
		player[num].speed = 0;

		player[num].direc = RIGHT;
		do {
			x_0 = rand() % MAX_X;
			y_0 = rand() % MAX_Y;
		} while (!is_floor(x_0, y_0));
		player[num].x = x_0;
		player[num].y = y_0;

		player[num].msg.data = malloc(MAX_NAME * sizeof(char));
		player[num].msg.disp = false;

		for (int i = 0; i < MAX_INV; i++) {
			player[num].inv[i].name = malloc(MAX_NAME * sizeof(char));
			player[num].inv[i].face = ' ';
			player[num].inv[i].color = 0;
			player[num].inv[i].map[0][0] = 0;
		}

		player[num].holding.name = malloc(MAX_NAME * sizeof(char));
		player[num].holding.face = ' ';
		player[num].holding.color = 0;
		player[num].holding.type = 0;
		player[num].holding.stat = 0;

		num++;

	} while (!feof(f));

	do {
		ch = fgetc(f);
		if (ch == '\n')
			nol++;
	} while (ch != EOF);
	playerqty = nol;

	fclose(f);

	free(name);

}

