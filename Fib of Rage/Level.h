#pragma once

#include "Scene.h"


class Level : public Scene
{
public:
	Level(int left, int right, int bottom, int top);
	Level();
	~Level();

	bool getInit() override;

	void render() override;
	void update(int deltaTime) override;
	void init(int player);

	void enableCollisionView(bool state);
	bool getCollisionView();
	void moveCamera(int left, int right, int bottom, int top);
	void restartLevel(int player);
	int getAttackers();
	void setAttackers(int value);

private:
	bool setBackground(const string &filename);
	void setAnimations();
	void setMainPlayer();

protected:
	TileMap *collision;
	Player *mainPlayer;
	int userPlayer;
	vector<Player *> characters;

	TexturedQuad *background;
	Texture tex;

	int numAttackers;

	bool showCollisions = false;

};

