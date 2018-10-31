#include <random>
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

#define OFFSET_X 90

#define MAX_ATTACKERS 2


enum  PLAYERS { RYU, HONDA, BISON };

enum UserAnims
{
	MAIN_SR, MAIN_SL, MAIN_MR, MAIN_ML, MAIN_KR, MAIN_KL, MAIN_PR, MAIN_PL, MAIN_SPR, MAIN_SPL, MAIN_HR, MAIN_HL, MAIN_DR, MAIN_DL
};

enum EnemyAnims
{
	ENE_SR, ENE_SL, ENE_MR, ENE_ML, ENE_PR, ENE_PL, ENE_HR, ENE_HL, ENE_DR, ENE_DL
};

enum StateEnemy 
{
	WAITING, MOVING, FIGHTING
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
		if (sprite->getAnimationFinished() || (sprite->animation() != MAIN_KL && sprite->animation() != MAIN_KR && sprite->animation() != MAIN_PR && sprite->animation() != MAIN_PL && sprite->animation() != MAIN_SPL && sprite->animation() != MAIN_SPR)) {
			if (!Game::instance().getKey(KEY_J) && !Game::instance().getKey(KEY_K) && !Game::instance().getKey(KEY_L)) {
				if (Game::instance().getKey(KEY_A))
				{
					if (sprite->animation() != MAIN_ML)
						sprite->changeAnimation(MAIN_ML);
					posPlayer.x -= 2;
					if (map->collisionMoveLeft(posPlayer, glm::ivec2(width_player, height_player)))
					{
						posPlayer.x += 2;
						sprite->changeAnimation(MAIN_SL);
					}
				}
				else if (Game::instance().getKey(KEY_D))
				{
					if (sprite->animation() != MAIN_MR)
						sprite->changeAnimation(MAIN_MR);
					posPlayer.x += 2;
					if (map->collisionMoveRight(posPlayer, glm::ivec2(width_player, height_player)))
					{
						posPlayer.x -= 2;
						sprite->changeAnimation(MAIN_SR);
					}
				}
				else if (Game::instance().getKey(KEY_S))
				{
					if (sprite->animation() == MAIN_SR || (sprite->getAnimationFinished() && sprite->animation() == MAIN_MR) || sprite->animation() == MAIN_KR || sprite->animation() == MAIN_PR || sprite->animation() == MAIN_SPR) {
						sprite->changeAnimation(MAIN_MR);
					}
					else if (sprite->animation() == MAIN_SL || (sprite->getAnimationFinished() && sprite->animation() == MAIN_ML) || sprite->animation() == MAIN_KL || sprite->animation() == MAIN_PL || sprite->animation() == MAIN_SPL) {
						sprite->changeAnimation(MAIN_ML);
					}
					posPlayer.y += 2;
					if (map->collisionMoveDown(posPlayer, glm::ivec2(width_player, height_player)))
					{
						posPlayer.y -= 2;
						if (sprite->animation() == MAIN_MR)
							sprite->changeAnimation(MAIN_SR);
						else if (sprite->animation() == MAIN_ML)
							sprite->changeAnimation(MAIN_SL);
					}
				}
				else if (Game::instance().getKey(KEY_W))
				{
					if (sprite->animation() == MAIN_SR || (sprite->getAnimationFinished() && sprite->animation() == MAIN_MR) || sprite->animation() == MAIN_KR || sprite->animation() == MAIN_PR || sprite->animation() == MAIN_SPR) {
						sprite->changeAnimation(MAIN_MR);
					}
					else if (sprite->animation() == MAIN_SL || (sprite->getAnimationFinished() && sprite->animation() == MAIN_ML) || sprite->animation() == MAIN_KL || sprite->animation() == MAIN_PL || sprite->animation() == MAIN_SPL) {
						sprite->changeAnimation(MAIN_ML);
					}
					posPlayer.y -= 2;
					if (map->collisionMoveUp(posPlayer, glm::ivec2(width_player, height_player)))
					{
						posPlayer.y += 2;
						if (sprite->animation() == MAIN_MR)
							sprite->changeAnimation(MAIN_SR);
						else if (sprite->animation() == MAIN_ML)
							sprite->changeAnimation(MAIN_SL);
					}
				}
				else
				{
					if (sprite->animation() == MAIN_ML || sprite->animation() == MAIN_KL || sprite->animation() == MAIN_PL || sprite->animation() == MAIN_SPL)
						sprite->changeAnimation(MAIN_SL);
					else if (sprite->animation() == MAIN_MR || sprite->animation() == MAIN_KR || sprite->animation() == MAIN_PR || sprite->animation() == MAIN_SPR)
						sprite->changeAnimation(MAIN_SR);
				}
			}
			if (Game::instance().getKey(KEY_K)) {
				if (sprite->animation() == MAIN_SL || sprite->animation() == MAIN_ML)
					sprite->changeAnimation(MAIN_KL);
				else if (sprite->animation() == MAIN_SR || sprite->animation() == MAIN_MR)
					sprite->changeAnimation(MAIN_KR);
			}
			else if (Game::instance().getKey(KEY_J)) {
				if (sprite->animation() == MAIN_SL || sprite->animation() == MAIN_ML)
					sprite->changeAnimation(MAIN_PL);
				else if (sprite->animation() == MAIN_SR || sprite->animation() == MAIN_MR)
					sprite->changeAnimation(MAIN_PR);
			}
			else if (Game::instance().getKey(KEY_L)) {
				if (sprite->animation() == MAIN_SL || sprite->animation() == MAIN_ML)
					sprite->changeAnimation(MAIN_SPL);
				else if (sprite->animation() == MAIN_SR || sprite->animation() == MAIN_MR)
					sprite->changeAnimation(MAIN_SPR);
			}
		}
	}
	else if (type_player == IA_PLAYER) {
		
		if (sprite->getAnimationFinished()) 
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			//This function creates a random number between 0-2 and is stored in dis(gen).
			// You can change the name of dis if you like. 
			std::uniform_int_distribution<> dis(0, 2);
			//gen(dis) -> This will generate 1 random numbers between 0-2
			int value = dis(gen);
			if (value == FIGHTING) {
				if (Game::instance().getAttackers() < MAX_ATTACKERS) {
					move_player_to_fight();
				}
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

void Player::setFighting()
{

	stateEnemy = FIGHTING;
}

void Player::move_player_to_fight()
{
	int x = mainPlayer->getPosition().x;
	if(posPlayer.x < x) positionToMove = glm::vec2(x - OFFSET_X, posPlayer.y);
	else positionToMove = glm::vec2(x + OFFSET_X, posPlayer.y);
}