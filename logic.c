#include "logic.h"
#include "draw.h"
#include "gba.h"

static PLAYER player1;
static BOSS boss1;
static int counter = 0;
static BULLET bullets[MAX_BULLET];
static int bulletSpawn[MAX_BULLET]; // tracks bullet spawn locations
static int bulletCount = 0;
static int dratioy = BULLET_SPAWN_Y - BOSS_SPAWN_Y;
int bossMove = 0;
void initPlayer(void) {
	player1.x = WIDTH / 2;
	player1.y = HEIGHT / 2;
	player1.velocity = 2;
	updatePlayer(player1.x, player1.y, -1 , -1);
}

void initBoss(void) {
	// TODO: Add in boss initalization animation
	boss1.x = BOSS_SPAWN_X;
	boss1.y = BOSS_SPAWN_Y; // these are really hard to understand, numbers are better
	boss1.velocity = 1;
	boss1.centerx = boss1.x + BWIDTH / 2;
	boss1.centery = boss1.y;
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
	// Below code is to only make the Boss move randomly in 1 direction once in a while
	counter = (counter + 1) % 30;
	if (counter == 0) {
		bossMove = randint(0, 3);
	}

	// Preventing the boss from getting too near screen edges
	else if (boss1.x > WIDTH - 20) {
		bossMove = 2;
	}
	else if (boss1.x < 20) {
		bossMove = 1;
	}
	oldx = boss1.x;
	switch (bossMove) {
		case 0:
			break;
		case 1:
			boss1.x = (boss1.x + boss1.velocity) % WIDTH;
			updateBoss(boss1.x, boss1.y, oldx, boss1.y);
			break;
		case 2:
			boss1.x = (boss1.x - boss1.velocity) % WIDTH;
			updateBoss(boss1.x, boss1.y, oldx, boss1.y);
			break;
	}
	boss1.centerx = boss1.x + BWIDTH / 2;
	updateBulletSpawn();
}

// Spaghetti code for bullet logic and spawning
void initBullets(void) {
	for (int i = 0; i < MAX_BULLET; i++) {
		bullets[i].exist = 0;
		bulletSpawn[i] = boss1.x - BULLET_SPAWN_REL + i*BULLET_SPAWN_GAP;
	}
}

void updateBulletSpawn(void) {
	for (int i = 0; i < MAX_BULLET; i++) {
		bulletSpawn[i] = boss1.x - BULLET_SPAWN_REL + i*BULLET_SPAWN_GAP;
	}
}

int updateBullets(void) {
	// updating each spawned bullet's position based on dratio
	for (int i = 0; i < MAX_BULLET; i++) {
		if (bullets[i].exist) {
			int oldx = bullets[i].x;
			int oldy = bullets[i].y;
			if (bullets[i].drc == dratioy) {
				int newx = bullets[i].x + bullets[i].dratio;
				// Getting rid of bullets that are about to go off screen
				if (newx < 5 || newx > WIDTH - 5) {
					bullets[i].exist = 0;
					bulletCount--;
					updateBullet(oldx, oldy, -1, -1);
					continue;
				}
				bullets[i].x = newx;
				bullets[i].drc = 0;
			}
			else {
				int newy = bullets[i].y + 1;
				if (newy < 5) {
					bullets[i].exist = 0;
					bulletCount--;
					updateBullet(oldx, oldy, -1, -1);
					continue;
				}
				bullets[i].y = newy;
				bullets[i].drc = bullets[i].drc + 1;
			}
			updateBullet(oldx, oldy, bullets[i].x, bullets[i].y);
		}
	}
	// randomly spawning new bullets at the spawns
	if (bulletCount < MAX_BULLET) {
		for (int i = 0; i < MAX_BULLET; i++) {
			int create = randint(0, 2);
			if (create && bullets[i].exist == 0) {
				bullets[i].exist = 1;
				bullets[i].drc = 0;
				bullets[i].dratio = bulletSpawn[i] - boss1.centerx;
				updateBullet(bulletSpawn[i], BULLET_SPAWN_Y, -1, -1);
				bulletCount++;
			}
		}
	}
	return 0;
}

