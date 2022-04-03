#include "draw.h"
#include "gba.h"
#include "logic.h"

#include "images/title.h"
#include "images/player.h"
void drawTitleScreen(void) {
	drawFullScreenImageDMA(title);
}

void drawGameBackground(void) {
	fillScreenDMA(BLUE);
	return;
}

void drawLoseScreen(void) {
	// TODO: Implement this
	return;
}

void drawInitPlayer(void) {
	drawImageDMA(50, 50, PLAYER_WIDTH, PLAYER_HEIGHT, player);
}

int updatePlayer(void) {
	// TODO: Implement this
	return 0;
}

int updateBullets(void) {
	// TODO: Implement this
	return 0;
}

void updateScore(void) {
	// TODO: Implement this
	return;
}
