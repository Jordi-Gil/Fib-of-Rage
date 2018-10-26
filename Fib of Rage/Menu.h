#pragma once

#include <iostream>
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
	void render();
	void init();
	void update(int deltaTime);

private:

	void initShaders();
	bool setBackground(const string &filename);

private:

	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	TexturedQuad *background;
	Texture tex;

public:
	int cameraLeft;
	int cameraRight;
	int cameraBottom;
	int cameraTop;
};

