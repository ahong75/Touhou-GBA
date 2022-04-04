#include "draw.h"
#include "gba.h"
#include "logic.h"
#include <stdio.h>

#include "images/title.h"
#include "images/player.h"
#include "images/boss.h"
#include "images/bullet.h"
#include "images/lose.h"
void drawTitleScreen(void) {
	drawFullScreenImageDMA(title);
}

void drawTitleMessage(void) {
	char message1[] = "PRESS ANY KEY";
	char message2[] = "TO START";
	drawString(HEIGHT / 4, WIDTH / 14, message1, WHITE);
	drawString(HEIGHT / 4 + 20, WIDTH / 14, message2, WHITE);
}
void drawGameBackground(void) {
	fillScreenDMA(BLUE);
	return;
}

void drawLoseScreen(long long score) {
	if (score != 0) {
		drawFullScreenImageDMA(lose);
		char message1[] = "YOU GOT HIT!";			
		char message2[] = "FINAL SCORE: ";
		char fscore[256];
		sprintf(fscore, "%lld", score);
		drawString(HEIGHT / 2 - 20, WIDTH / 2 - 40, message1, WHITE);
		drawString(HEIGHT / 2, WIDTH / 2 - 40, message2, WHITE);
		drawCenteredString(HEIGHT / 2 + 20, WIDTH / 2 - 30, 40, 10, fscore, WHITE);
		return;	
	}
}

int updatePlayer(int nrow, int ncol, int orow, int ocol) {
	if (orow != -1) {
		drawRectDMA(orow, ocol, PWIDTH, PHEIGHT, BLUE);
	}
	drawImageDMA(nrow, ncol, PWIDTH, PHEIGHT, player);
	return 0;
}

// There's some bug where the boss leaves a couple pixels behind
int updateBoss(int nrow, int ncol, int orow, int ocol) {
	if (orow != -1) {
		drawRectDMA(orow, ocol, BWIDTH + 1, BHEIGHT + 1, BLUE);
	}
	drawImageDMA(nrow, ncol, BWIDTH, BHEIGHT, boss);
	return 0;
}

int updateBullet(int nrow, int ncol, int orow, int ocol) {
	if (orow != -1) {
		drawRectDMA(orow, ocol, buWIDTH + 1, buHEIGHT + 1, BLUE);
	}
	if (nrow != -1) {
		drawImageDMA(nrow, ncol, buWIDTH, buHEIGHT, bullet);
	}
	return 0;
}

void drawScore(long long score) {
	char cur[256];
	sprintf(cur, "%lld", score); // converting long long to str
	if (score != 1) {
		char prev[256];
		sprintf(prev, "%lld", score - 1);
		drawString(5, WIDTH - 30, prev, BLUE);
	}
	drawString(5, WIDTH - 30, cur, BLACK);
}
