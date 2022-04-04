#ifndef DRAW_H
#define DRAW_H
// Draws the title screen for the game
void drawTitleScreen(void);

// Draws title message
void drawTitleMessage(void);

// Draws the background for the actual gameplay
void drawGameBackground(void);

// Draws the lose screen after the player dies
void drawLoseScreen(long long score);

// Updates the number shown on the screen representing score
void updateScore(void);

// Updates the player/bullets and checks if they have collided with a bullet
void updatePlayer(int nrow, int ncol, int orow, int ocol);

// Updates the boss
void updateBoss(int nrow, int ncol, int orow, int ocol);

// Update the bullets and checks if they have collided with the player
void updateBullet(int nrow, int ncol, int orow, int ocol);

// Updates a laser
void updateLaser(int nrow, int ncol, int orow, int ocol);

// Draws damage animation
void drawDamage(int row, int col);

// Draw the player's current score
void drawScore(long long score);
#endif