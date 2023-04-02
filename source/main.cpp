#include <3ds.h>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include "ui.h"
#include "audio.h"

void InitServices() {
    osSetSpeedupEnable(true);
    ptmuInit();
    romfsInit();
    gfxInitDefault();
    ndspInit(); 
}

void ExitServices() {
    gfxExit(); 
    ptmuExit();
    romfsExit();
}

int main(int argc, char **argv) {
    // Use New3DS CPU speedup

    InitServices();

    std::string TEST_AUDIO = "romfs:/m.wav";

    PrintConsole topScreen, bottomScreen;

    consoleInit(GFX_TOP, &topScreen);
    consoleInit(GFX_BOTTOM, &bottomScreen);

    // The top screen has 30 rows and 50 columns
    // The bottom screen has 30 rows and 40 columns
    Screen topScr(Coord(50, 30), &topScreen);
    Screen bottomScr(Coord(40, 30), &bottomScreen);

    UI topUI(&topScr);
    UI bottomUI(&bottomScr);

    
    size_t a = 0;

    Menu mainm(ColorText("-- Main Menu", ""), std::vector<MENU_PAIR>({
            {ColorText("File", ""), [](){}}, 
            {ColorText("Audio Test", ""), [&TEST_AUDIO, &topUI](){play_wav(TEST_AUDIO); return;}},
            {ColorText("Exit", "\x1b[31m"), [](){ExitServices(); exit(0);}}, 
    }));


    bottomUI.draw_menu(mainm, a);
    bottomUI.update();
    size_t x = 0;

    topUI.render_text(&x, 2, ColorText("Uta3ds Revisited (ver0);", ""));
    x = 0;
    topUI.render_text(&x, 3, ColorText("Copyright 2023, FurryR, OKMT, Serix", ""));
    x = 0;
    topUI.update();

    // Main loop
    while (aptMainLoop()) {
        hidScanInput();
        // hidKeysDown returns information about which buttons have been just
        // pressed (and they weren't in the previous frame)
        u32 kDown = hidKeysDown();

        if (kDown & KEY_A) {
            mainm.items[a].second();
        } 
		if (kDown & KEY_UP) {
            if (a > 0) a--;
            else a = mainm.items.size() - 1;
            bottomUI.draw_menu(mainm, a);
            bottomUI.update();  
        }
        if (kDown & KEY_DOWN) {
            if (a < mainm.items.size() - 1) a++;
            else a = 0;
            bottomUI.draw_menu(mainm, a);
            bottomUI.update();  
        }

        topUI.render_menubar();
        topUI.update();

        
        // Flush and swap framebuffers
        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    ExitServices();

    return 0;
}
