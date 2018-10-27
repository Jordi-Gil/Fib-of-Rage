#pragma once

#include <iostream>
#include <utility> 
#include "TexturedQuad.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "Scene.h"


class Menu : public Scene
{
public:
	Menu(int left, int right, int bottom, int top);
	Menu();
	~Menu();
	void update(int deltaTime) override;
	void render() override;
	void init() override;

private:

	bool setBackground(const string &filename);

private:

	int antDeltaTime = 0;
	int numberAnims = 4;
	Texture texture;
	Sprite *background;
};

