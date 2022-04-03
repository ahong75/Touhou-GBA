#include "draw.h"
#include "gba.h"
#include "logic.h"

#include "images/title.h"
#include "images/player.h"
#include "images/boss.h"
#include "images/bullet.h"
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

int updatePlayer(int nrow, int ncol, int orow, int ocol) {
	if (orow != -1) {
		drawRectDMA(orow, ocol, PWIDTH, PHEIGHT, BLUE);
	}
	drawImageDMA(nrow, ncol, PWIDTH, PHEIGHT, player);
	return 0;
}

int updateBoss(int nrow, int ncol, int orow, int ocol) {
	if (orow != -1) {
		drawRectDMA(orow, ocol, BWIDTH, BHEIGHT, BLUE);
	}
	drawImageDMA(nrow, ncol, BWIDTH, BHEIGHT, boss);
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
