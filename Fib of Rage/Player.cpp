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
#define MOVE_SPEED 2;

#define OFFSET_X 90

#define MAX_ATTACKERS 2
#define GAP_UNTIL_MOVE 25

#define P_WIDTH 90*2
#define P_HEIGHT 120*2

enum Players
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

enum StateEnemy 
{
	WAITING, MOVING, FIGHTING, HITTED
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
					if (map->collisionMoveLeft(posPlayer, glm::ivec2(width_player, height_player)) || posPlayer.x <= min_x)
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
					if (map->collisionMoveRight(posPlayer, glm::ivec2(width_player, height_player)) || posPlayer.x > max_x)
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
			if (stateEnemy != HITTED) {
				if (!freeChooseDest) {
					gotoDestination();
					if (map->collisionMoveUp(posPlayer, glm::ivec2(width_player, height_player))) 
						posPlayer.y += 2;
					else if (map->collisionMoveDown(posPlayer, glm::ivec2(width_player, height_player))) 
						posPlayer -= 2;
					if (map->collisionMoveLeft(posPlayer, glm::ivec2(width_player, height_player)) ) 
						posPlayer.x += 2;
					else if (map->collisionMoveRight(posPlayer, glm::ivec2(width_player, height_player)) ) 
						posPlayer.x -= 2;
					
					if (direction && stateEnemy != WAITING) 
						sprite->changeAnimation(ENE_MR);
					else if(!direction && stateEnemy != WAITING) 
						sprite->changeAnimation(ENE_ML);
					else if (direction && stateEnemy == WAITING) 
						sprite->changeAnimation(ENE_SR);
					else
						sprite->changeAnimation(ENE_SL);
				}
				else if (stateEnemy == WAITING) {
					if (direction) sprite->changeAnimation(ENE_SL);
					else sprite->changeAnimation(ENE_SR);
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

void Player::move_player_to_fight()
{
	if (freeChooseDest) {
		int x = mainPlayer->getPosition().x;
		int y = mainPlayer->getPosition().y;
		if (posPlayer.x < x) {
			positionToMove = glm::vec2(x - OFFSET_X, y - P_HEIGHT);
			direction = false; // Izquierda
		}
		else {
			positionToMove = glm::vec2(x + OFFSET_X, y - P_HEIGHT);
			direction = true; // Derecha
		}
		
		freeChooseDest = false;
	}
}

void Player::move_around_player()
{
	if (freeChooseDest) {
		std::random_device rd;
		std::mt19937 gen(rd());
		//This function creates a random number between 0-n and is stored in dis(gen).
		// You can change the name of dis if you like. 
		int n1 = max_x - min_x;
		std::uniform_int_distribution<> disX(0, n1);
		int n2 = max_y - min_y;
		std::uniform_int_distribution<> disY(0, n2);
		//gen(dis) -> This will generate 1 random numbers between 0-1
		int valueX = disX(gen);
		int valueY = disY(gen);
		positionToMove.x = valueX + min_x;
		positionToMove.y = valueY + min_y;

		positionToMove.y -= P_HEIGHT;

		freeChooseDest = false;
	}
}

void Player::gotoDestination()
{

	int temp_x = posPlayer.x;
	int temp_y = posPlayer.y;

	if (temp_x == positionToMove.x && temp_y == positionToMove.y) {
		positionToMove.x = 0;
		positionToMove.y = 0;
	}

	if (positionToMove.x == 0 && positionToMove.y == 0)
	{
		if (timeHitted == GAP_UNTIL_MOVE) {
			stateEnemy = WAITING;
			freeChooseDest = true;
		}
		else {
			timeHitted++;
		}
		return;
	}

	stateEnemy = MOVING;

	if (direction) {

		if (temp_x - 2 > positionToMove.x) {
			temp_x -= 2;
		}
		else if (temp_x - 2 <= positionToMove.x) {
			temp_x -= temp_x - positionToMove.x;
			stateEnemy = WAITING;
		}
	}
	else {
		if (temp_x + 2 < positionToMove.x) {
			temp_x += 2;
		}
		else if (temp_x + 2 >= positionToMove.x) {
			temp_x += positionToMove.x - posPlayer.x;
			stateEnemy = WAITING;
		}
	}


	if (positionToMove.y > temp_y) {
		freeChooseDest = false;
		if (temp_y + 2 < positionToMove.y) {
			temp_y += 2;
		}
		else {
			temp_y += positionToMove.y - posPlayer.y;
			stateEnemy = WAITING;
		}

		if (positionToMove.y < temp_y) {
			freeChooseDest = false;
			if (posPlayer.y - 2 > positionToMove.y) {
				posPlayer.y -= 2;
			}
			else {
				posPlayer -= positionToMove.y - posPlayer.y;
				stateEnemy = WAITING;
			}
		}
	}

	posPlayer.x = temp_x;
	posPlayer.y = temp_y;
}

void Player::changeState()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	//This function creates a random number between 0-2 and is stored in dis(gen).
	// You can change the name of dis if you like. 
	std::uniform_int_distribution<> dis(0, 2);
	//gen(dis) -> This will generate 1 random numbers between 0-1
	int value = dis(gen);
	if (value == MOVING) {
		stateEnemy = MOVING;
		move_around_player();
	}
	else if (value == FIGHTING) {
		stateEnemy = FIGHTING;
		move_player_to_fight();
	}
	else stateEnemy = WAITING;
}

void Player::set_X_max_min(int x_max, int x_min)
{
	max_x = x_max;
	min_x = x_min;
}