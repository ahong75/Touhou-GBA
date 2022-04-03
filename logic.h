#ifndef LOGIC_H
#define LOGIC_H

#define PHEIGHT 20
#define PWIDTH 20
typedef struct player {
	int x;
	int y;
	int velocity;
} PLAYER;

typedef struct bullet {
	int topleft;
	int bottomright;
	int velocity;
} BULLET;
// Can designate an enemy "center" and use the slopes from the center to the edge to generate a bullet

typedef struct boss {
	int x;
	int y;
	int velocity;
	int center;
} BOSS;
void initPlayer(void);
void movePlayer(void);

#endif