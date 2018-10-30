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

	void setrenderMenu();

private:

	void setMenu(const string &filename);
	void setCtrl(const string &filename);
	void setCred(const string &filename);
	void setPlayers(const string &filename1, const string &filename2, const string &filename3);

private:

	int antDeltaTime = 0;
	int numberAnims = 4;
	Texture texture, contrTex, credTex, playerTex[3];
	TexturedQuad *credits, *controls, *players;
	Sprite *menu;

	int index;

	bool renderMenu = true, renderCont = false, renderCred = false, renderPlayers = false;

	bool finishOneTime = false;
};

