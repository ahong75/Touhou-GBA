#ifndef LOGIC_H
#define LOGIC_H
#include "images/title.h"
#include "images/player.h"
#include "images/boss.h"
#include "images/bullet.h"
#include "images/lose.h"
// These are really pointless when they're already basically defined in another header file
#define PHEIGHT PLAYER_HEIGHT // height of player
#define PWIDTH PLAYER_WIDTH // width of player
#define PSPEED 3 // speed of player
#define BWIDTH BOSS_WIDTH // width of boss
#define BHEIGHT BOSS_HEIGHT // height of boss
#define buWIDTH BULLET_WIDTH // width of bullet
#define buHEIGHT BULLET_HEIGHT // height of bullet
#define MAX_BULLET 5 // max amount of bullets allowed at a time
#define BULLET_SPAWN_Y 40 // initial height of bullet spawn
#define BULLET_SPAWN_REL 20 // relative y distance from boss center for the first bullet
#define BULLET_SPAWN_GAP 10 // gap size between bullet spawn locations
#define BOSS_SPAWN_X 120 // initial x location of boss spawn
#define BOSS_SPAWN_Y 8 // initial y location of boss spawn
#define NEGMOD(x, y, M) (((((x) - (y)) % M) + (M)) % M) // needed to handle undefined mod of negative numbers
typedef struct player {
	int x;
	int y;
	int center;
	int velocity;
} PLAYER;

// TODO: Moving bullets at angles is difficult because floating point math is not allowed
typedef struct bullet {
	int exist;
	int x;
	int y;
	int velocity;
	int homing;
} BULLET;
// Can designate an enemy "center" and use the slopes from the center to the edge to generate a bullet

typedef struct boss {
	int x;
	int y;
	int velocity;
	int centerx;
	int centery;
} BOSS;
void initPlayer(void);
void initBoss(void);
void initBullets(void);
void moveSprites(void);
void updateBulletSpawn(void);
void updateScore(void);
void loseScreen(void);
int updateBullets(void); // Initalizes and moves bullets


#endif