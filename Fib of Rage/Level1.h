#pragma once

#include "Level.h"

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
	/*
	void enableCollisionView(bool state);
	bool getCollisionView();
	*/
private:
	bool setBackground(const string &filename);
	void setAnimations();
	void setMainPlayer();

protected:

	TileMap *collisions;
	Player *mainPlayer;
	int userPlayer;
	vector<Player *> characters;

	TexturedQuad *background, *life_bar, *yellow_bar;
	Texture texBack, tex_life, tex_yellow;

	int numAttackers;

	

	int max_x;
	int min_x;

};

