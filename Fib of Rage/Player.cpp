#include <cmath>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"

#define USER_PLAYER 0
#define IA_PLAYER 1

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

#define KEY_J 106
#define KEY_K 107
#define KEY_L 108
#define KEY_W 119
#define KEY_A 97
#define KEY_S 115
#define KEY_D 100

#define SPEED 8

enum RyuAnims
{
	RYU_SL, RYU_SR, RYU_ML, RYU_MR, RYU_KR, RYU_KL, RYU_PR, RYU_PL, RYU_HR, RYU_HL
};

enum BisonAnims
{
	BIS_SL, BIS_SR, BIS_ML, BIS_MR, BIS_KR, BIS_KL, BIS_PR, BIS_PL, BIS_SPR, BIS_SPL
};

enum HondaAnims
{
	HON_SL, HON_SR, HON_ML, HON_MR, HON_KR, HON_KL, HON_PR, HON_PL, HON_SPR, HON_SPL
};

enum EnemyAnims
{
	ENE_SL, ENE_SR, ENE_ML, ENE_MR, ENE_PR, ENE_PL
};



void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, const string &filename, vector<pair<int, vector<glm::vec2>>> &animations, glm::ivec2 tam, glm::vec2 prop, int type, Player *player)
{
	bJumping = false;
	width_player = tam.x;
	height_player = tam.y;
	type_player = type;
	mainPlayer = player;
	spritesheet.loadFromFile(filename, TEXTURE_PIXEL_FORMAT_RGBA);
	
	sprite = Sprite::createSprite(tam, prop, &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(animations.size());
	
	for (unsigned int i = 0; i < animations.size(); i++)
	{
		int anim = animations[i].first;
		sprite->setAnimationSpeed(anim, SPEED);
		for (unsigned int j = 0; j < animations[i].second.size(); j++) {
			sprite->addKeyframe(anim, animations[i].second[j]);
		}
	}

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if(type_player == USER_PLAYER){
		if (sprite->getAnimationFinished() || (sprite->animation() != RYU_KL && sprite->animation() != RYU_KR && sprite->animation() != RYU_PR && sprite->animation() != RYU_PL && sprite->animation() != RYU_HL && sprite->animation() != RYU_HR)) {
			if (!Game::instance().getKey(KEY_J) && !Game::instance().getKey(KEY_K) && !Game::instance().getKey(KEY_L)) {
				if (Game::instance().getKey(KEY_A))
				{
					if (sprite->animation() != RYU_ML)
						sprite->changeAnimation(RYU_ML);
					posPlayer.x -= 2;
					if (map->collisionMoveLeft(posPlayer, glm::ivec2(width_player, height_player)))
					{
						posPlayer.x += 2;
						sprite->changeAnimation(RYU_SL);
					}
				}
				else if (Game::instance().getKey(KEY_D))
				{
					if (sprite->animation() != RYU_MR)
						sprite->changeAnimation(RYU_MR);
					posPlayer.x += 2;
					if (map->collisionMoveRight(posPlayer, glm::ivec2(width_player, height_player)))
					{
						posPlayer.x -= 2;
						sprite->changeAnimation(RYU_SR);
					}
				}
				else if (Game::instance().getKey(KEY_S))
				{
					if (sprite->animation() == RYU_SR || (sprite->getAnimationFinished() && sprite->animation() == RYU_MR) || sprite->animation() == RYU_KR || sprite->animation() == RYU_PR || sprite->animation() == RYU_HR) {
						sprite->changeAnimation(RYU_MR);
					}
					else if (sprite->animation() == RYU_SL || (sprite->getAnimationFinished() && sprite->animation() == RYU_ML) || sprite->animation() == RYU_KL || sprite->animation() == RYU_PL || sprite->animation() == RYU_HL) {
						sprite->changeAnimation(RYU_ML);
					}
					posPlayer.y += 2;
					if (map->collisionMoveDown(posPlayer, glm::ivec2(width_player, height_player)))
					{
						posPlayer.y -= 2;
						if (sprite->animation() == RYU_MR)
							sprite->changeAnimation(RYU_SR);
						else if (sprite->animation() == RYU_ML)
							sprite->changeAnimation(RYU_SL);
					}
				}
				else if (Game::instance().getKey(KEY_W))
				{
					if (sprite->animation() == RYU_SR || (sprite->getAnimationFinished() && sprite->animation() == RYU_MR) || sprite->animation() == RYU_KR || sprite->animation() == RYU_PR || sprite->animation() == RYU_HR) {
						sprite->changeAnimation(RYU_MR);
					}
					else if (sprite->animation() == RYU_SL || (sprite->getAnimationFinished() && sprite->animation() == RYU_ML) || sprite->animation() == RYU_KL || sprite->animation() == RYU_PL || sprite->animation() == RYU_HL) {
						sprite->changeAnimation(RYU_ML);
					}
					posPlayer.y -= 2;
					if (map->collisionMoveUp(posPlayer, glm::ivec2(width_player, height_player)))
					{
						posPlayer.y += 2;
						if (sprite->animation() == RYU_MR)
							sprite->changeAnimation(RYU_SR);
						else if (sprite->animation() == RYU_ML)
							sprite->changeAnimation(RYU_SL);
					}
				}
				else
				{
					if (sprite->animation() == RYU_ML || sprite->animation() == RYU_KL || sprite->animation() == RYU_PL || sprite->animation() == RYU_HL)
						sprite->changeAnimation(RYU_SL);
					else if (sprite->animation() == RYU_MR || sprite->animation() == RYU_KR || sprite->animation() == RYU_PR || sprite->animation() == RYU_HR)
						sprite->changeAnimation(RYU_SR);
				}
			}
			if (Game::instance().getKey(KEY_K)) {
				if (sprite->animation() == RYU_SL || sprite->animation() == RYU_ML)
					sprite->changeAnimation(RYU_KL);
				else if (sprite->animation() == RYU_SR || sprite->animation() == RYU_MR)
					sprite->changeAnimation(RYU_KR);
			}
			else if (Game::instance().getKey(KEY_J)) {
				if (sprite->animation() == RYU_SL || sprite->animation() == RYU_ML)
					sprite->changeAnimation(RYU_PL);
				else if (sprite->animation() == RYU_SR || sprite->animation() == RYU_MR)
					sprite->changeAnimation(RYU_PR);
			}
			else if (Game::instance().getKey(KEY_L)) {
				if (sprite->animation() == RYU_SL || sprite->animation() == RYU_ML)
					sprite->changeAnimation(RYU_HL);
				else if (sprite->animation() == RYU_SR || sprite->animation() == RYU_MR)
					sprite->changeAnimation(RYU_HR);
			}
		}
	}
	else if (type_player == IA_PLAYER) {
		if (sprite->getAnimationFinished() || (sprite->animation() != ENE_PL && sprite->animation() != ENE_PR)) {
			if (sprite->animation() != ENE_ML && sprite->animation() != ENE_MR)
				sprite->changeAnimation(ENE_ML);
			posPlayer.x -= 2;

			if (map->collisionMoveLeft(posPlayer, glm::ivec2(width_player, height_player)))
			{
				posPlayer.x += 2;
				sprite->changeAnimation(ENE_SL);
			}
		}
	}


	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::ivec2 Player::getPosition()
{
	return posPlayer;
}


/*if(bJumping)
{
jumpAngle += JUMP_ANGLE_STEP;
if(jumpAngle == 180)
{
bJumping = false;
posPlayer.y = startY;
}
else
{
posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
if(jumpAngle > 90)
bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
}
}
else
{
posPlayer.y += FALL_STEP;
if(map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
{
if(Game::instance().getSpecialKey(GLUT_KEY_UP))
{
bJumping = true;
jumpAngle = 0;
startY = posPlayer.y;
}
}
}*/