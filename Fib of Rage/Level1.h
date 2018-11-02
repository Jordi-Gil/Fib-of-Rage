#ifndef LEVEL
#define LEVEL
#include "Level.h"
#endif


class Level1 : public Level
{
public:
	Level1(int left, int right, int bottom, int top);
	Level1();
	~Level1();

	void render() override;
	void update(int deltaTime) override;
	void init(int player);

	void restartLevel(int player);
	void moveCamera(int left, int right, int bottom, int top);
	//void acabaNivel();
	
private:
	bool setBackground(const string &filename);
	void setAnimations();
	void setMainPlayer();
	void setBossAnimations();
	void avanzaLevel();

protected:

	TileMap *collisions;
	Player *mainPlayer;
	Player *bossPlayer;
	int userPlayer;
	vector<Player *> characters;

	TexturedQuad *background, *life_bar, *yellow_bar;
	Texture texBack, tex_life, tex_yellow;

	int numAttackers; //cada ronda

	int points = 0;
	Text pointsText;
	Text texGo;

	int max_x;
	int min_x;

	bool renderBoss = false;

};

