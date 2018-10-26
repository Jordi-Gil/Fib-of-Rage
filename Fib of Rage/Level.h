#pragma once

#include "Scene.h"


class Level : public Scene
{
public:
	Level(int left, int right, int bottom, int top);
	Level();
	~Level();

	void render() override;
	void update(int deltaTime) override;

	void init() override;

	void enableCollisionView(bool state) override;
	bool getCollisionView() override;
	void moveCamera(int left, int right, int bottom, int top) override;

private:
	bool setBackground(const string &filename);

protected:
	FullMap fullMap;
	Player *mainPlayer;
	TexturedQuad *background;
	Texture tex;
	bool showCollisions = false;

};

