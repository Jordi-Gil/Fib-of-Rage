#pragma once
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <algorithm>
#include "Scene.h"

#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 250

#define P_WIDTH 90*2
#define P_HEIGHT 120*2

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 547

#define USER_PLAYER 0
#define IA_PLAYER 1

enum PLAYERS
{
	RYU, HONDA, BISON
};

enum UserAnims
{
	MAIN_SR, MAIN_SL, MAIN_MR, MAIN_ML, MAIN_KR, MAIN_KL, MAIN_PR, MAIN_PL, MAIN_SPR, MAIN_SPL, MAIN_HR, MAIN_HL, MAIN_DR, MAIN_DL
};

enum EnemyAnims
{
	ENE_SR, ENE_SL, ENE_MR, ENE_ML, ENE_PR, ENE_PL, ENE_HR, ENE_HL, ENE_DR, ENE_DL
};

enum BossAnims
{
	BOSS_SR, BOSS_SL, BOSS_MR, BOSS_SL, BOSS_PSR, BOSS_PSL, BOSS_PHR, BOSS_PHL, BOSS_HR, BOSS_HL, BOSS_DR, BOSS_DL
};

class Level : public Scene
{
public:
	Level(int left, int right, int bottom, int top);
	Level();
	~Level();

	void render() override;
	void update(int deltaTime) override;
	void init();


	virtual void enableCollisionView(bool state) final;
	virtual bool getCollisionView() final;

protected:
	bool showCollisions = false;
};

