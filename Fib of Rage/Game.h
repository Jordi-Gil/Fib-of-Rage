#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#ifndef SCENE
#define SCENE
#include "Scene.h"
#endif

#ifndef LEVEL1
#define LEVEL1
#include "Level1.h"
#endif

#ifndef LEVEL2
#define LEVEL2
#include "Level2.h"
#endif

#ifndef MENU_
#define MENU_
#include "Menu.h"
#endif


#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 547

enum Scenes { MENU, LEVEL_1, LEVEL_2 };

class Game
{

public:
	Game()
	{
		gameScenes[MENU] = new Menu(0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, 0);
		gameScenes[LEVEL_1] = new Level1(0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, 0);
		gameScenes[LEVEL_2] = new Level2(0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, 0);
	}
	
	
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	
	bool getKey(int key) const;
	bool getSpecialKey(int key) const;

	void changeScene(int value, int player);
	void setPlayer(int player);
	int getPlayer();


	void exitGame();

private:
	bool bPlay;
	
	Scene **gameScenes = new Scene *[3];


	bool keys[256], specialKeys[256];
	int scene = MENU;
	int selected_player;
};


#endif // _GAME_INCLUDE


