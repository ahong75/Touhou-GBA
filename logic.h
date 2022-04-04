#ifndef LOGIC_H
#define LOGIC_H

// These are really pointless when they're already basically defined in another header file
#define PHEIGHT 20
#define PWIDTH 20
#define BWIDTH 15
#define BHEIGHT 29
#define buWIDTH 10
#define buHEIGHT 10
#define MAX_BULLET 10
#define BULLET_SPAWN_Y 45
#define BULLET_SPAWN_REL 30
#define BULLET_SPAWN_GAP 7
#define BOSS_SPAWN_X 120
#define BOSS_SPAWN_Y 8
typedef struct player {
	int x;
	int y;
	int velocity;
} PLAYER;

typedef struct bullet {
	int exist;
	int x;
	int y;
	int dratio; // "direction-ratio" if the bullet goes BULLET_SPAWN_Y - BOSS_SPAWN_Y units downwards, it should move dratio[1] in the x direction
	int drc; // direction-ratio count -> tracks the number of times moved downwards 
	int velocity;
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
int updateBullets(void); // Initalizes and moves bullets

#endif