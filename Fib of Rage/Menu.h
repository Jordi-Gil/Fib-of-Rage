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

	bool setBackground(vector<string> filenames, const string &base);

private:

	TexturedQuad *background;

	int numberAnims = 4;
	int currentAnim;
	int texture = 0;
	float timeAnimation = 0.f;
	vector<pair<vector<Texture>, float>> animations;

};

