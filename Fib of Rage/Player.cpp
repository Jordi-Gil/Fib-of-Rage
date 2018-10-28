#include <cmath>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

#define WIDTH_PLAYER 90*2
#define HEIGHT_PLAYER 120*2

#define KEY_J 106
#define KEY_K 107
#define KEY_L 108
#define KEY_W 119
#define KEY_A 97
#define KEY_S 115
#define KEY_D 100

#define SPEED 8

enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, KICK_RIGHT, KICK_LEFT, PUNCH_RIGHT, PUNCH_LEFT, HADOUKEN_RIGHT, HADOUKEN_LEFT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, const string &filename, vector<pair<int, vector<glm::vec2>>> &animations)
{
	bJumping = false;
	spritesheet.loadFromFile(filename, TEXTURE_PIXEL_FORMAT_RGBA);
	
	sprite = Sprite::createSprite(glm::ivec2(WIDTH_PLAYER, HEIGHT_PLAYER), glm::vec2(0.05f, 0.5f), &spritesheet, &shaderProgram);
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
	if (sprite->getAnimationFinished() || (sprite->animation() != KICK_LEFT && sprite->animation() != KICK_RIGHT && sprite->animation() != PUNCH_RIGHT && sprite->animation() != PUNCH_LEFT && sprite->animation() != HADOUKEN_LEFT && sprite->animation() != HADOUKEN_RIGHT)) {
		if(!Game::instance().getKey(KEY_J) && !Game::instance().getKey(KEY_K) && !Game::instance().getKey(KEY_L)){
			if (Game::instance().getKey(KEY_A))
			{
				if (sprite->animation() != MOVE_LEFT)
					sprite->changeAnimation(MOVE_LEFT);
				posPlayer.x -= 2;
				if (map->collisionMoveLeft(posPlayer, glm::ivec2(WIDTH_PLAYER, HEIGHT_PLAYER)))
				{
					posPlayer.x += 2;
					sprite->changeAnimation(STAND_LEFT);
				}
			}
			else if (Game::instance().getKey(KEY_D))
			{
				if (sprite->animation() != MOVE_RIGHT)
					sprite->changeAnimation(MOVE_RIGHT);
				posPlayer.x += 2;
				if (map->collisionMoveRight(posPlayer, glm::ivec2(WIDTH_PLAYER, HEIGHT_PLAYER)))
				{
					posPlayer.x -= 2;
					sprite->changeAnimation(STAND_RIGHT);
				}
			}
			else if (Game::instance().getKey(KEY_S))
			{
				if (sprite->animation() == STAND_RIGHT || (sprite->getAnimationFinished() && sprite->animation() == MOVE_RIGHT) || sprite->animation()== KICK_RIGHT || sprite->animation() == PUNCH_RIGHT || sprite->animation() == HADOUKEN_RIGHT) {
					sprite->changeAnimation(MOVE_RIGHT);
				}
				else if (sprite->animation() == STAND_LEFT || (sprite->getAnimationFinished() && sprite->animation() == MOVE_LEFT) || sprite->animation() == KICK_LEFT || sprite->animation() == PUNCH_LEFT || sprite->animation() == HADOUKEN_LEFT) {
					sprite->changeAnimation(MOVE_LEFT);
				}
				posPlayer.y += 2;
				if (map->collisionMoveDown(posPlayer, glm::ivec2(WIDTH_PLAYER, HEIGHT_PLAYER)))
				{
					posPlayer.y -= 2;
					if (sprite->animation() == MOVE_RIGHT)
						sprite->changeAnimation(STAND_RIGHT);
					else if (sprite->animation() == MOVE_LEFT)
						sprite->changeAnimation(STAND_LEFT);
				}
			}
			else if (Game::instance().getKey(KEY_W))
			{
				if (sprite->animation() == STAND_RIGHT || (sprite->getAnimationFinished() && sprite->animation() == MOVE_RIGHT) || sprite->animation() == KICK_RIGHT || sprite->animation() == PUNCH_RIGHT || sprite->animation() == HADOUKEN_RIGHT) {
					sprite->changeAnimation(MOVE_RIGHT);
				}
				else if (sprite->animation() == STAND_LEFT || (sprite->getAnimationFinished() && sprite->animation() == MOVE_LEFT) || sprite->animation() == KICK_LEFT || sprite->animation() == PUNCH_LEFT || sprite->animation() == HADOUKEN_LEFT) {
					sprite->changeAnimation(MOVE_LEFT);
				}
				posPlayer.y -= 2;
				if (map->collisionMoveUp(posPlayer, glm::ivec2(WIDTH_PLAYER, HEIGHT_PLAYER)))
				{
					posPlayer.y += 2;
					if (sprite->animation() == MOVE_RIGHT)
						sprite->changeAnimation(STAND_RIGHT);
					else if (sprite->animation() == MOVE_LEFT)
						sprite->changeAnimation(STAND_LEFT);
				}
			}
			else
			{
				if (sprite->animation() == MOVE_LEFT || sprite->animation() == KICK_LEFT || sprite->animation() == PUNCH_LEFT || sprite->animation() == HADOUKEN_LEFT)
					sprite->changeAnimation(STAND_LEFT);
				else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == KICK_RIGHT || sprite->animation() == PUNCH_RIGHT || sprite->animation() == HADOUKEN_RIGHT)
					sprite->changeAnimation(STAND_RIGHT);
			}
		}
		if (Game::instance().getKey(KEY_K)) {
			if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT)
				sprite->changeAnimation(KICK_LEFT);
			else if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT)
				sprite->changeAnimation(KICK_RIGHT);
		}
		else if (Game::instance().getKey(KEY_J)) {
			if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT)
				sprite->changeAnimation(PUNCH_LEFT);
			else if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT)
				sprite->changeAnimation(PUNCH_RIGHT);
		}
		else if (Game::instance().getKey(KEY_L)) {
			if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT)
				sprite->changeAnimation(HADOUKEN_LEFT);
			else if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT)
				sprite->changeAnimation(HADOUKEN_RIGHT);
		}
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
