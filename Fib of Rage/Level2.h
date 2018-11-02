#pragma once

#include "Level.h"

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

