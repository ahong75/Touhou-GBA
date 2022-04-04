#include "main.h"
#include "draw.h"
#include "gba.h"
#include "logic.h"

#include <stdio.h>
#include <stdlib.h>

enum gba_state {
  START,
  TITLE,
  INIT_PLAY,
  PLAY,
  // WIN,
  LOSE,
};

int main(void) {
  // Manipulate REG_DISPCNT here to set Mode 3. //
  REG_DISPCNT = 0x403;
  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  // Load initial application state
  enum gba_state state = START;

  // Create player struct
  // PLAYER player;

  while (1) {
    currentButtons = BUTTONS; // Load the current state of the buttons

    if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
      state = START;
    }
    switch (state) {
      case START:
        waitForVBlank();
        drawTitleScreen();
        state = TITLE; // go to title screen after startup
        break;
      case TITLE:
        // if press anykey -> go to PLAY
        drawTitleMessage();
        if (KEY_JUST_PRESSED(ANY_KEY, currentButtons, previousButtons)) {
          state = INIT_PLAY;
        }
        break;
      case INIT_PLAY: // we have these init states to prevent tearing
        // Set up play screen and then go to PLAY
        waitForVBlank();
        drawGameBackground();
        waitForVBlank();
        initPlayer();
        waitForVBlank();
        initBoss();
        initBullets(); // Just sets up the bullet structs in the array
        state = PLAY;
        break;
      case PLAY:
        waitForVBlank();
        moveSprites();
        updateLasers();
        updateScore();
        if (updateBullets()) {
          state = LOSE;
        }
        break;
      // case WIN:
      //   break;
      case LOSE:
        // TODO: if player presses start -> retry (go back to init_play)
        loseScreen();
        break;
    }

    previousButtons = currentButtons; // Store the current state of the buttons
  }

  UNUSED(previousButtons); // You can remove this once previousButtons is used

  return 0;
}
