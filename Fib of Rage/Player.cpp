#include <cmath>
#include <iostream>
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

enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, KICK_RIGHT, KICK_LEFT, PUNCH_RIGHT, PUNCH_LEFT, HADOUKEN_RIGHT, HADOUKEN_LEFT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	spritesheet.loadFromFile("Resources/Sprites/ryu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	
	sprite = Sprite::createSprite(glm::ivec2(WIDTH_PLAYER, HEIGHT_PLAYER), glm::vec2(0.05f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(10);
	
	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.55f, 0.5f));
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.50f, 0.5f));
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.45f, 0.5f));
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.40f, 0.5f));
	
	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.40f, 0.0f));
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.45f, 0.0f));
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.50f, 0.0f));
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.55f, 0.0f));
	
	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.95f, 0.5f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.90f, 0.5f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.85f, 0.5f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.80f, 0.5f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.75f, 0.5f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.05f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.10f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.15f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.20f, 0.0f));

	sprite->setAnimationSpeed(KICK_RIGHT, 8);
	sprite->addKeyframe(KICK_RIGHT, glm::vec2(0.60f, 0.0f));
	sprite->addKeyframe(KICK_RIGHT, glm::vec2(0.65f, 0.0f));
	sprite->addKeyframe(KICK_RIGHT, glm::vec2(0.70f, 0.0f));

	sprite->setAnimationSpeed(KICK_LEFT, 8);
	sprite->addKeyframe(KICK_LEFT, glm::vec2(0.35f, 0.5f));
	sprite->addKeyframe(KICK_LEFT, glm::vec2(0.30f, 0.5f));
	sprite->addKeyframe(KICK_LEFT, glm::vec2(0.25f, 0.5f));

	sprite->setAnimationSpeed(PUNCH_RIGHT, 8);
	sprite->addKeyframe(PUNCH_RIGHT, glm::vec2(0.25f, 0.0f));
	sprite->addKeyframe(PUNCH_RIGHT, glm::vec2(0.30f, 0.0f));
	sprite->addKeyframe(PUNCH_RIGHT, glm::vec2(0.35f, 0.0f));

	sprite->setAnimationSpeed(PUNCH_LEFT, 8);
	sprite->addKeyframe(PUNCH_LEFT, glm::vec2(0.60f, 0.5f));
	sprite->addKeyframe(PUNCH_LEFT, glm::vec2(0.65f, 0.5f));
	sprite->addKeyframe(PUNCH_LEFT, glm::vec2(0.70f, 0.5f));

	sprite->setAnimationSpeed(HADOUKEN_RIGHT, 8);
	sprite->addKeyframe(HADOUKEN_RIGHT, glm::vec2(0.75f, 0.0f));
	sprite->addKeyframe(HADOUKEN_RIGHT, glm::vec2(0.80f, 0.0f));
	sprite->addKeyframe(HADOUKEN_RIGHT, glm::vec2(0.85f, 0.0f));
	sprite->addKeyframe(HADOUKEN_RIGHT, glm::vec2(0.90f, 0.0f));
	sprite->addKeyframe(HADOUKEN_RIGHT, glm::vec2(0.95f, 0.0f));

	sprite->setAnimationSpeed(HADOUKEN_LEFT, 8);
	sprite->addKeyframe(HADOUKEN_LEFT, glm::vec2(0.20f, 0.5f));
	sprite->addKeyframe(HADOUKEN_LEFT, glm::vec2(0.15f, 0.5f));
	sprite->addKeyframe(HADOUKEN_LEFT, glm::vec2(0.10f, 0.5f));
	sprite->addKeyframe(HADOUKEN_LEFT, glm::vec2(0.05f, 0.5f));
	sprite->addKeyframe(HADOUKEN_LEFT, glm::vec2(0.00f, 0.5f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if(Game::instance().getKey(KEY_A))
	{
		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= 2;
		if(map->collisionMoveLeft(posPlayer, glm::ivec2(WIDTH_PLAYER, HEIGHT_PLAYER)))
		{
			posPlayer.x += 2;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if(Game::instance().getKey(KEY_D))
	{
		if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += 2;
		if(map->collisionMoveRight(posPlayer, glm::ivec2(WIDTH_PLAYER, HEIGHT_PLAYER)))
		{
			posPlayer.x -= 2;
			sprite->changeAnimation(STAND_RIGHT);
		}
	}
	else if (Game::instance().getKey(KEY_S))
	{
		posPlayer.y += 2;
		if (map->collisionMoveDown(posPlayer, glm::ivec2(WIDTH_PLAYER, HEIGHT_PLAYER)))
		{
			posPlayer.y -= 2;
		}
	}
	else if (Game::instance().getKey(KEY_W))
	{
		posPlayer.y -= 2;
		if (map->collisionMoveUp(posPlayer, glm::ivec2(WIDTH_PLAYER, HEIGHT_PLAYER)))
		{
			posPlayer.y += 2;
		}
	}
	else if (Game::instance().getKey(KEY_K)) {
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
	else if (Game::instance().getKey(KEY_L)){
		if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(HADOUKEN_LEFT);
		else if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(HADOUKEN_RIGHT);
	}
	else
	{
		if(sprite->animation() == MOVE_LEFT || sprite->animation() == KICK_LEFT || sprite->animation() == PUNCH_LEFT || sprite->animation() == HADOUKEN_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if(sprite->animation() == MOVE_RIGHT || sprite->animation() == KICK_RIGHT || sprite->animation() == PUNCH_RIGHT || sprite->animation() == HADOUKEN_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
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




