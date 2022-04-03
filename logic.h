#ifndef LOGIC_H
#define LOGIC_H

typedef struct player {
	int topleft;
	int bottomright;
	int velocity;
} PLAYER;

typedef struct bullet {
	int topleft;
	int bottomright;
	int velocity;
} BULLET;

#endif