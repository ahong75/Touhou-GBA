#include "logic.h"
#include "draw.h"
#include "gba.h"

static PLAYER player1;
static BOSS boss1;
void initPlayer(void) {
	player1.x = WIDTH / 2;
	player1.y = HEIGHT / 2;
	player1.velocity = 2;
	updatePlayer(player1.x, player1.y, -1 , -1);
}

void initBoss(void) {
	// TODO: Add in boss initalization animation
	boss1.x = WIDTH / 2;
	boss1.y = HEIGHT / 8;
	boss1.velocity = 1;
	updateBoss(boss1.x, boss1.y, -1, -1);
}
/**
#define BUTTON_RIGHT    (1<<4)
#define BUTTON_LEFT     (1<<5)
#define BUTTON_UP       (1<<6)
#define BUTTON_DOWN     (1<<7)
**/
void moveSprites(void) {
	int oldx = player1.x;
	int oldy = player1.y;
	if (KEY_DOWN(BUTTON_LEFT, BUTTONS)) {
		player1.x = (player1.x - player1.velocity) % WIDTH;
	}
	if (KEY_DOWN(BUTTON_RIGHT, BUTTONS)) {
		player1.x = (player1.x + player1.velocity) % WIDTH;
	}
	if (KEY_DOWN(BUTTON_UP, BUTTONS)) {
		player1.y = (player1.y - player1.velocity) % HEIGHT;
	}
	if (KEY_DOWN(BUTTON_DOWN, BUTTONS)) {
		player1.y = (player1.y + player1.velocity) % HEIGHT;
	}
	updatePlayer(player1.x, player1.y, oldx, oldy);

	
}

void updateBullets(void) {

}