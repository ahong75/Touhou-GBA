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
static int bossHealth = BOSS_INITIAL_HEALTH;
static LASER lasers[MAX_LASER];

int bossMove = 0;

void initPlayer(void) {
	score = 0;
	player1.x = WIDTH / 2;
	player1.y = HEIGHT / 2;
	player1.velocity = PSPEED;
	player1.center = player1.x + PWIDTH / 2;
	updatePlayer(player1.x, player1.y, -1 , -1);
	for (int i = 0; i < MAX_LASER; i++) {
		lasers[i].exist = 0;
	}
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

extra:
Player bullets
boss patterns
cleanup defines like the width and height 
cleanup logic
music
sprites instead of images for bullets at least
**/
void moveSprites(void) {
	int oldx = player1.x;
	int oldy = player1.y;
	if (KEY_DOWN(BUTTON_LEFT, BUTTONS)) {
		player1.x = NEGMOD(player1.x, player1.velocity, WIDTH);
	}
	if (KEY_DOWN(BUTTON_RIGHT, BUTTONS)) {
		player1.x = (player1.x + player1.velocity) % WIDTH;
	}
	if (KEY_DOWN(BUTTON_UP, BUTTONS)) {
		player1.y = NEGMOD(player1.y, player1.velocity, HEIGHT);
	}
	if (KEY_DOWN(BUTTON_DOWN, BUTTONS)) {
		player1.y = (player1.y + player1.velocity) % HEIGHT;
	}
	updatePlayer(player1.x, player1.y, oldx, oldy);
	player1.center = (player1.x + PWIDTH / 2) % WIDTH;
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

void updateLasers(void) {
		int update = 0; // Can only create one bullet per call; Prevents creating too many bullets at once
		if (counter % 5 == 0) {
			update = 1;
		}
		for (int i = 0; i < MAX_LASER; i++) {
			if (lasers[i].exist) {
				int oldx = lasers[i].x;
				int oldy = lasers[i].y;
				if (lasers[i].y < 5) {
					lasers[i].exist = 0;
					updateLaser(-1, -1, oldx, oldy);
				}
				else {
					lasers[i].y = lasers[i].y - lasers[i].velocity;
					updateLaser(lasers[i].x, lasers[i].y, oldx, oldy);
					if (boss1.x <= lasers[i].x &&
						lasers[i].x < boss1.x + BWIDTH &&
						lasers[i].y < boss1.y + BHEIGHT) {
						bossHealth -= 10;
						score += 50;
						lasers[i].exist = 0;
						updateLaser(-1, -1, lasers[i].x, lasers[i].y);
						drawDamage(lasers[i].x, lasers[i].y);
					}
				}
			}
			if (update == 1 && lasers[i].exist == 0 && KEY_DOWN(BUTTON_A, BUTTONS)) {
				lasers[i].exist = 1;
				lasers[i].x = player1.center - 1;
				lasers[i].y = player1.y - 5;
				lasers[i].velocity = LASER_VELOCITY;
				updateLaser(lasers[i].x, lasers[i].y, -1, -1);
				update = 0;
			}
		}
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
		bullets[i].homing = 0;
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
			if (bullets[i].homing) {
				if (oldx < player1.x && oldy < player1.y) {
					newx = newx + bullets[i].velocity;
				}
				else if (oldx > player1.x && oldy < player1.y) {
					newx = newx - bullets[i].velocity;
				}
			}
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
				return 1; // transition to lose state
			}
		}
	}
	// randomly spawning new bullets at the spawns
	int create = 0;
	if (counter == 0) {
		create = randint(0, 4); // 0 is no bullets, 1, 2, 3 are different bullet modes
	}
	if (score >= 100) {
		if (bulletCount < MAX_BULLET) {
			for (int i = 0; i < MAX_BULLET; i++) {
				if (bullets[i].exist == 0) {
					bullets[i].homing = 0;
					bullets[i].y = BULLET_SPAWN_Y;
					bullets[i].velocity = 1;
					if (create == 2) {
						bullets[i].exist = 1;
						bullets[i].x = bulletSpawn[MAX_BULLET / 2];
						bulletCount++;
						updateBullet(bullets[i].x, bullets[i].y, -1, -1);
						break;
					}
					if (create == 1 || create == 3) {
						int spawn = randint(0, 2);
						if (spawn) {
							bullets[i].exist = 1;
							bullets[i].x = bulletSpawn[i];
							bullets[i].velocity = randint(1, 3);
							if (create == 3) {
								bullets[i].velocity = 1;
								bullets[i].homing = 1;
							}
							bulletCount++;
							updateBullet(bullets[i].x, bullets[i].y, -1, -1);
						}
					}
				}
			}
		}
	}
	return 0;
}

