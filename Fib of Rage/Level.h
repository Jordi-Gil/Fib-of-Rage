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
	void init() override;

	void enableCollisionView(bool state);
	bool getCollisionView();
	void moveCamera(int left, int right, int bottom, int top);
	void restartLevel();

private:
	bool setBackground(const string &filename);

protected:
	FullMap fullMap;
	Player *mainPlayer;
	Player *enemy[1];

	TexturedQuad *background;
	Texture tex;

	bool showCollisions = false;

};

