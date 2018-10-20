#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	spritesheets[0].loadFromFile("Resources/Sprites/idle_ryu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheets[1].loadFromFile("Resources/Sprites/moving_ryu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprites[0] = Sprite::createSprite(glm::ivec2(2*90, 2*120), glm::vec2(0.25, 1), &spritesheets[0], &shaderProgram);
	sprites[0]->setNumberAnimations(4);
	
	sprites[0]->setAnimationSpeed(STAND_LEFT, 8);
	sprites[0]->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));
	sprites[0]->addKeyframe(STAND_LEFT, glm::vec2(0.25f, 0.f));
	sprites[0]->addKeyframe(STAND_LEFT, glm::vec2(0.50f, 0.f));
	sprites[0]->addKeyframe(STAND_LEFT, glm::vec2(0.75f, 0.f));
		
	sprites[0]->setAnimationSpeed(STAND_RIGHT, 8);
	sprites[0]->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
		
	sprites[0]->setAnimationSpeed(MOVE_LEFT, 8);
	sprites[0]->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprites[0]->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.f));
	sprites[0]->addKeyframe(MOVE_LEFT, glm::vec2(0.50f, 0.f));
	sprites[0]->addKeyframe(MOVE_LEFT, glm::vec2(0.75f, 0.f));
		
	sprites[0]->setAnimationSpeed(MOVE_RIGHT, 8);
	sprites[0]->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
	sprites[0]->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.f));
	sprites[0]->addKeyframe(MOVE_RIGHT, glm::vec2(0.50f, 0.f));
	sprites[0]->addKeyframe(MOVE_RIGHT, glm::vec2(0.75f, 0.f));
		
	sprites[0]->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprites[0]->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
	sprites[0]->update(deltaTime);
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if(sprites[0]->animation() != MOVE_LEFT)
			sprites[0]->changeAnimation(MOVE_LEFT);
		posPlayer.x -= 2;
		/*if(map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x += 2;
			sprite->changeAnimation(STAND_LEFT);
		}*/
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if(sprites[0]->animation() != MOVE_RIGHT)
			sprites[0]->changeAnimation(MOVE_RIGHT);
		posPlayer.x += 2;
		/*if(map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x -= 2;
			sprite->changeAnimation(STAND_RIGHT);
		}*/
	}
	else
	{
		if(sprites[0]->animation() == MOVE_LEFT)
			sprites[0]->changeAnimation(STAND_LEFT);
		else if(sprites[0]->animation() == MOVE_RIGHT)
			sprites[0]->changeAnimation(STAND_RIGHT);
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
	
	sprites[0]->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprites[0]->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprites[0]->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}




