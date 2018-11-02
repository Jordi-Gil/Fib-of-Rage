#ifndef LEVEL
#define LEVEL
#include "Level.h"
#endif

class Level2 : public Level
{
public:
	Level2(int left, int right, int bottom, int top);
	Level2();
	~Level2();

	void render() override;
	void update(int deltaTime) override;
	void init(int player);

	void restartLevel(int player);
	void moveCamera(int left, int right, int bottom, int top);

	void avanzaLevel();

private:
	bool setBackground(const string &filename);
	void setAnimations();
	void setMainPlayer();
	void setBossAnimations();

protected:

	TileMap *collisions;
	Player *mainPlayer;
	Player *bossPlayer;
	int userPlayer;
	vector<Player *> characters;

	TexturedQuad *background, *life_bar, *yellow_bar;
	Texture texBack, tex_life, tex_yellow;

	int numAttackers; //Cada ronda
	
	int points;
	Text pointsText;
	Text texGo;

	int max_x;
	int min_x;

	bool renderBoss;

};

