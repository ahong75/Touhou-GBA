#ifndef DRAW_H
#define DRAW_H
// Draws the title screen for the game
void drawTitleScreen(void);

// Draws the background for the actual gameplay
void drawGameBackground(void);

// Draws the lose screen after the player dies
void drawLoseScreen(void);

// Updates the number shown on the screen representing score
void updateScore(void);

// Updates the player/bullets and checks if they have collided with a bullet
int updatePlayer(int nrow, int ncol, int orow, int ocol);

// Updates the boss
int updateBoss(int nrow, int ncol, int orow, int ocol);

// Update the bullets and checks if they have collided with the player
int updateBullet(int nrow, int ncol, int orow, int ocol);

// Draw the player's current score
void drawScore(long long score);
#endif