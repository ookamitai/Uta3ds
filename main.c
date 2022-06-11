#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

struct Menu{
	char name[30];
	char context[7][30];
	int item;
};

void flushConsole(){
	printf("\x1b[0;0H");
	for(int j = 0; j < 960; j++){
		printf(" ");
	}
	printf("\x1b[0;0H");
}

void debugConsole(char* debugcontent){
	printf("\x1b[32;0H\r                              ");
	printf("\x1b[32;0H%s", debugcontent);
	printf("\x1b[0;0H");
}

void dirList(){
	flushConsole();
	struct dirent *de;  // Pointer for directory entry
  
    // opendir() returns a pointer of DIR type. 
    DIR *dr = opendir(".");
  
    if (dr == NULL)  // opendir returns NULL if couldn't open directory
    {
        debugConsole("DirReadFailure");
        exit(0);
    }
  
    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html
    // for readdir()
    while ((de = readdir(dr)) != NULL){
        printf("%s\n", de->d_name);}
    closedir(dr);
	debugConsole("Press B to return.");
	while(aptMainLoop()){
		hidScanInput();
		u32 kDown = hidKeysDown();
		if (kDown & KEY_B) break;
	gfxFlushBuffers();
	gfxSwapBuffers();
	gspWaitForVBlank();    
}
}

void exitS(){
	// Exit services
	gfxExit();
	exit(0);
}

void printMenu(struct Menu m){
	printf("%s\n\n", m.name);
	for(int k = 0; k < 7; k++){
			if(k == m.item){
				printf("  \x1b[47;30m->%s\x1b[0m\n", m.context[k]);
			} else {
			printf("->%s\n", m.context[k]);
			}
	}
}


int main(int argc, char **argv)
{
	gfxInitDefault();

	struct Menu mainScreen = {"Main Menu", {"Open", "New", "Help", "About", "empty", "empty", "Exit"}, 0};

	PrintConsole topScreen, bottomScreen;
	consoleInit(GFX_TOP, &topScreen);
	consoleInit(GFX_BOTTOM, &bottomScreen);//init both consoles
	consoleSelect(&topScreen);
	printf("\n\n\n\n\n\n\n\n\n\n\n         _    _ _        ____      _     \n");
	printf("         | |  | | |      |___ \\    | | \n");   
	printf("         | |  | | |_ __ _  __) | __| |___ \n");
	printf("         | |  | | __/ _` ||__ < / _` / __|\n");
	printf("         | |__| | || (_| |___) | (_| \\__ \\\n");
	printf("          \\____/ \\__\\__,_|____/ \\__,_|___/\n");

	consoleSelect(&bottomScreen);//pretty self-explanatory, i think
	debugConsole("Splash Drawn");
	printf("Uta3ds ver0.1\n");
	printf("Developed by ookamitai & Serix\n");
	printf("\x1b[13;10HPress Start to exit\n");
	printf("\x1b[14;10HPress A to continue");
	debugConsole("Text Loaded");
	// Main loop
	while (aptMainLoop()){
		hidScanInput();
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START) exitS();
		if (kDown & KEY_A) break;
		gfxFlushBuffers();
		gfxSwapBuffers();
		//Wait for VBlank
		gspWaitForVBlank();
	}
	
	//This is main screen part.
	while (aptMainLoop()){
		//consoleInit(GFX_BOTTOM, &bottomScreen);
		flushConsole();
		hidScanInput();
		u32 kDown = hidKeysDown();
		if (kDown & KEY_UP) {
			if(mainScreen.item > 0){
				mainScreen.item -= 1;
			} else {
				mainScreen.item = 6;
			}
		}

		if (kDown & KEY_DOWN) {
			if(mainScreen.item < 6){
				mainScreen.item += 1;
			} else {
				mainScreen.item = 0;
			}
		}
		
		if (kDown & KEY_A){
			switch (mainScreen.item)
			{
			case 6:
				exitS();
				break;
			case 0:
				dirList();
				break;
			default:
				debugConsole("MenuIndexError");
				break;
			}
		}

		printMenu(mainScreen);
		gfxFlushBuffers();
		gfxSwapBuffers();
		gspWaitForVBlank();
	}

	return 0;
}
