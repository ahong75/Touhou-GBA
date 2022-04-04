#include "logic.h"
#include "draw.h"
#include "gba.h"

static PLAYER player1;
static BOSS boss1;
static int counter = 0;
static BULLET bullets[MAX_BULLET];
static int bulletSpawn[MAX_BULLET]; // tracks bullet spawn locations
static int bulletCount = 0;
static long long score = 0;

int bossMove = 0;

void initPlayer(void) {
	score = 0;
	player1.x = WIDTH / 2;
	player1.y = HEIGHT / 2;
	player1.velocity = PSPEED;
	player1.center = player1.x + PWIDTH / 2;
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
TODO:
Collision with player 
Scoreboard using vBlankCounter
Collision between bullets
Text for menu and death screen

extra:
cleanup defines like the width and height 
cleanup logic
music
sprites instead of images for bullets at least
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
	player1.center = player1.x + PWIDTH / 2;
	// Below code is to only make the Boss move randomly in 1 direction once in a while
	counter = (counter + 1) % 30;
	if (counter == 0) {
		bossMove = randint(0, 3);
	}
	// Preventing the boss from getting too near screen edges
	else if (boss1.x > WIDTH - 40) {
		bossMove = 2;
	}
	else if (boss1.x < 40) {
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

void updateScore(void) {
	score++;
	drawScore(score);
}

void loseScreen(void) {
	drawLoseScreen(score);
	score = 0;
}

// Spaghetti code for bullet logic and spawning
void initBullets(void) {
	bulletCount = 0;
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
	// updating each spawned bullet's position based on its relative position to the player
	for (int i = 0; i < MAX_BULLET; i++) {
		if (bullets[i].exist) {
			int oldx = bullets[i].x;
			int oldy = bullets[i].y;
			int newx = bullets[i].x;
			// if (oldx < player1.x && oldy < player1.y) {
			// 	newx = newx + bullets[i].velocity;
			// }
			// else if (oldx > player1.x && oldy < player1.y) {
			// 	newx = newx - bullets[i].velocity;
			// }
			int newy = bullets[i].y + bullets[i].velocity;
			if (newy > HEIGHT - 5 || newx < 5 || newx > WIDTH - 5) {
				bullets[i].exist = 0;
				bulletCount--;
				updateBullet(-1, -1, oldx, oldy);
				continue;
			}
			bullets[i].x = newx;
			bullets[i].y = newy;
			updateBullet(bullets[i].x, bullets[i].y, oldx, oldy);
			// collision detection between bullets
			for (int j = 0; j < MAX_BULLET; j++) {
				if (i != j && bullets[j].exist) {
					if (bullets[i].x < bullets[j].x + buWIDTH &&
						bullets[i].x + buWIDTH > bullets[j].x &&
						bullets[i].y < bullets[j].y + buHEIGHT &&
						bullets[i].y + buHEIGHT > bullets[j].y) {
						// collision detected between bullets
						updateBullet(-1, -1, bullets[i].x, bullets[i].y);
						bullets[i].exist = 0;
						bulletCount--;
						break;
					}
				}
			}
			// collision detection with player
			if (bullets[i].x < player1.center &&
				bullets[i].x + buWIDTH > player1.center &&
				bullets[i].y < player1.y &&
				bullets[i].y + buHEIGHT > player1.y) {
				return 1; // transition to the next state
			}
		}
	}
	// TODO: Only launch bullets after a certain amount of time has passed
	// randomly spawning new bullets at the spawns
	if (bulletCount < MAX_BULLET) {
		for (int i = 0; i < MAX_BULLET; i++) {
			int create = randint(0, 2);
			if (create && bullets[i].exist == 0) {
				bullets[i].exist = 1;
				bullets[i].x = bulletSpawn[i];
				bullets[i].y = BULLET_SPAWN_Y;
				bullets[i].velocity = randint(1, 3);
				updateBullet(bullets[i].x, bullets[i].y, -1, -1);
				bulletCount++;
			}
		}
	}
	return 0;
}

