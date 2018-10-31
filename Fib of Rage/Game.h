#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE

#include "Scene.h"
#include "Level.h"
#include "Menu.h"


#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 547

enum Scenes { MENU, LEVEL_1 };

class Game
{

public:
	Game()
	{
		gameScenes[MENU] = new Menu(0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, 0);
		gameScenes[LEVEL_1] = new Level(0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, 0);
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

	void exitGame();
	int getAttackers();
	void setAttackers(int value);

private:
	bool bPlay;
	
	Scene **gameScenes = new Scene *[2];


	Level level;
	bool keys[256], specialKeys[256];
	int scene = MENU;

};


#endif // _GAME_INCLUDE


