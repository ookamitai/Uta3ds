#include <3ds.h>

#include <iostream>

#include "ui.h"

int main(int argc, char **argv) {
    // Use New3DS CPU speedup
    osSetSpeedupEnable(true);
    gfxInitDefault();
    PrintConsole topScreen, bottomScreen;

    consoleInit(GFX_TOP, &topScreen);
    consoleInit(GFX_BOTTOM, &bottomScreen);

    // The top screen has 30 rows and 50 columns
    // The bottom screen has 30 rows and 40 columns
    Screen topScr(Coord(30, 50), &topScreen);
    Screen bottomScr(Coord(30, 40), &bottomScreen);

    UI topUI(&topScr);
    UI bottomUI(&bottomScr);

    bottomUI.render_log(ColorText("not okay", "").output());
    bottomUI.update();
	int a = 11;
    // Main loop
    while (aptMainLoop()) {
        hidScanInput();
        // hidKeysDown returns information about which buttons have been just
        // pressed (and they weren't in the previous frame)
        u32 kDown = hidKeysDown();
        if (kDown & KEY_B) break;  // break in order to return to hbmenu
		if (kDown & KEY_A) {
			a--;
			bottomUI.render_log(ColorText(std::to_string(a), "").output());
    	    bottomUI.update();
		}
        // Flush and swap framebuffers
        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    gfxExit();

    return 0;
}
