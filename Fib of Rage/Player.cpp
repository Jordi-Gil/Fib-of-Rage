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

#define OFFSET_X 50

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
	WAITING, MOVING, MOVING_TO_FIGHT, FIGHTING, HITTED
};

enum Orientation
{
	LEFT, RIGHT
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
				if (sprite->animation() == MAIN_SL || sprite->animation() == MAIN_ML)//Abadede 35 a 110 colision ; 50 a 75
				{
					sprite->changeAnimation(MAIN_KL);
				}
				else if (sprite->animation() == MAIN_SR || sprite->animation() == MAIN_MR)
				{
					sprite->changeAnimation(MAIN_KR);
				}
			}
			else if (Game::instance().getKey(KEY_J)) {
				Game::instance().keyReleased(KEY_J);
				if (sprite->animation() == MAIN_SL || sprite->animation() == MAIN_ML)
				{
					sprite->changeAnimation(MAIN_PL);
					checkCollisions();
				}
				else if (sprite->animation() == MAIN_SR || sprite->animation() == MAIN_MR)
				{
					sprite->changeAnimation(MAIN_PR);
					checkCollisions();
				}
			}
			else if (Game::instance().getKey(KEY_L)) {
				if (sprite->animation() == MAIN_SL || sprite->animation() == MAIN_ML)
				{
					sprite->changeAnimation(MAIN_SPL);
				}
				else if (sprite->animation() == MAIN_SR || sprite->animation() == MAIN_MR)
				{
					sprite->changeAnimation(MAIN_SPR);
				}
			}
		}
	}
	else if (type_player == IA_PLAYER) {
		
		if (stateEnemy == MOVING || stateEnemy == MOVING_TO_FIGHT) {
			gotoDestination();

			if (map->collisionMoveUp(posPlayer, glm::ivec2(width_player, height_player)))
				posPlayer.y += speed_player;
			else if (map->collisionMoveDown(posPlayer, glm::ivec2(width_player, height_player)))
				posPlayer.y -= speed_player;
			if (map->collisionMoveLeft(posPlayer, glm::ivec2(width_player, height_player)))
				posPlayer.x += speed_player;
			else if (map->collisionMoveRight(posPlayer, glm::ivec2(width_player, height_player)))
				posPlayer.x -= speed_player;

		}

		if (posPlayer.x < mainPlayer->getPosition().x) orientation = RIGHT;
		else orientation = LEFT;

		if (sprite->getAnimationFinished()) {
			if (orientation == RIGHT && (stateEnemy == MOVING || stateEnemy == MOVING_TO_FIGHT))
				sprite->changeAnimation(ENE_MR);
			else if (orientation == LEFT && (stateEnemy == MOVING || stateEnemy == MOVING_TO_FIGHT))
				sprite->changeAnimation(ENE_ML);
			else if (orientation == RIGHT && stateEnemy == WAITING)
				sprite->changeAnimation(ENE_SR);
			else if (orientation == LEFT && stateEnemy == WAITING)
				sprite->changeAnimation(ENE_SL);
		}

		if (orientation == RIGHT && stateEnemy == HITTED)
		{
			sprite->changeAnimation(ENE_HR);
			stateEnemy = WAITING;
		}
		else if (orientation == LEFT && stateEnemy == HITTED)
		{
			sprite->changeAnimation(ENE_HL);
			stateEnemy = WAITING;
		}
	}
	
	// Quitar IF
	if(type_player == USER_PLAYER) sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
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
	if (freeChooseDest && timeHitted == GAP_UNTIL_MOVE) {
		stateEnemy = MOVING_TO_FIGHT;
		is_moving = true;
		int x = mainPlayer->getPosition().x;
		int y = mainPlayer->getPosition().y;
		if (posPlayer.x < x) {
			positionToMove = glm::vec2(x - OFFSET_X, y);
			orientation = RIGHT; // DERECHA
		}
		else {
			positionToMove = glm::vec2(x + OFFSET_X, y);
			orientation = LEFT; // IZQUIERDA
		}
		freeChooseDest = false;
	}
	else if (timeHitted < GAP_UNTIL_MOVE) 
		timeHitted++;
}

void Player::move_around_player()
{
	if (freeChooseDest && timeHitted == GAP_UNTIL_MOVE) {
		stateEnemy = MOVING;
		is_moving = true;

		std::random_device rd;
		std::uniform_int_distribution<> disX(min_x, max_x);
		std::uniform_int_distribution<> disY(min_y, max_y);
		
		
		positionToMove.x = disX(rd);
		positionToMove.y = disY(rd);

		freeChooseDest = false;
	}
	else if (timeHitted < GAP_UNTIL_MOVE) 
		timeHitted++;
}

void Player::gotoDestination()
{
	glm::ivec2 temp;
	temp.x = posPlayer.x;
	temp.y = posPlayer.y;

	if (stateEnemy == HITTED) return;

	if (temp.x == positionToMove.x && temp.y == positionToMove.y) {
		positionToMove.x = 0;
		positionToMove.y = 0;
		is_moving = false;
		
		return;
	}

	
	if (temp.x < positionToMove.x) 
	{
		if (temp.x + speed_player < positionToMove.x)
		{
			temp.x += speed_player;
		}
		else
		{
			temp.x = positionToMove.x;
		}
	}
	else if (temp.x > positionToMove.x) 
	{
		if (temp.x - speed_player > positionToMove.x) 
		{
			temp.x -= speed_player;
		}
		else if (temp.x - speed_player <= positionToMove.x)
		{
			temp.x = positionToMove.x;
		}
	}

	if (temp.y < positionToMove.y)
	{
		if (temp.y + speed_player < positionToMove.y)
		{
			temp.y += speed_player;
		}
		else if (temp.y + speed_player >= positionToMove.y)
		{
			temp.y = positionToMove.y;
		}
	}
	
	if (temp.y > positionToMove.y)
	{
		if (temp.y - speed_player > positionToMove.y)
		{
			temp.y -= speed_player;
		}
		else
		{
			temp.y = positionToMove.y;
		}
	}

	posPlayer.x = temp.x;
	posPlayer.y = temp.y;
}

void Player::changeState()
{
	std::random_device rd;
	std::uniform_int_distribution<int> dis(0,2);
	int value = dis(rd);
	if (value == MOVING) {
		move_around_player();
	}
	else if (value == MOVING_TO_FIGHT) {
		move_player_to_fight();
	}
	else {
		if (!is_moving) {
			stateEnemy = WAITING;
			std::uniform_int_distribution<int> dis2(0, 1500);
			if (dis2(rd) < 50)
				freeChooseDest = true;
		}
	}
	
}

void Player::set_X_max_min(int x_max, int x_min)
{
	max_x = x_max;
	min_x = x_min;
}

void Player::setHitted()
{
	stateEnemy = HITTED;
	timeHitted = 0;
	positionToMove = glm::ivec2(0,0);
}

void Player::checkCollisions()
{
	for each(Player *p in enemies)
	{
		//Abadede 35 a 110 colision ; 50 a 75
		//puño (70 , 47) ; (95 , 53) 
		// (minx1 < maxx2) && (minx2 < maxx1)
		// (miny1 < maxy2) && (miny2 < maxy1)
		int minx1, miny1, maxx1, maxy1, minx2, miny2, maxx2, maxy2;
		minx1 = posPlayer.x + 70; miny1 = posPlayer.y + 47;
		maxx1 = posPlayer.x + 95; maxy1 = posPlayer.y + 53;
		if (   p->getPosition().x + 75) && (p->getPosition().x + 50 < posPlayer.x + 95)) && 
			 (( < p->getPosition().y + 110) && (p->getPosition().y + 35 < posPlayer.y + 53))) 
		{
			p->setHitted();
		}
	}
}

void Player::setEnemies(const vector<Player *> &enemies)
{

	for each(Player *p in enemies) {
		this->enemies.push_back(p);
	}

}