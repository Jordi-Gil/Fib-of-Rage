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

enum Orientation
{
	LEFT, RIGHT
};

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, const string &filename, vector<pair<int, vector<glm::vec2>>> &animations, glm::ivec2 tam, glm::vec2 prop, int type, Player *player, CharType charType, int life)
{
	hp = life;
	bJumping = false;
	width_player = tam.x;
	height_player = tam.y;
	type_player = type;
	mainPlayer = player;
	this->charType = charType;
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

		if (is_dead && !one_time) 
		{
			one_time = true;
			if (orientation == RIGHT)
				sprite->changeAnimation(MAIN_DR);
			else sprite->changeAnimation(MAIN_DL);
		}
		else if(is_dead && one_time)
		{
			sprite->setPosition(glm::vec2(-120,0));
		}
		if (is_hitted) {
			if (orientation == RIGHT) {
				sprite->changeAnimation(MAIN_HR);
				is_hitted = false;
			}
			else
			{
				sprite->changeAnimation(MAIN_HL);
				is_hitted = false;
			}
		}
		else if (sprite->getAnimationFinished() || (sprite->animation() != MAIN_KL && sprite->animation() != MAIN_KR && sprite->animation() != MAIN_PR && sprite->animation() != MAIN_PL && sprite->animation() != MAIN_SPL && sprite->animation() != MAIN_SPR && sprite->animation() != MAIN_HR && sprite->animation() != MAIN_HL && sprite->animation() != MAIN_DR && sprite->animation() != MAIN_DL)) {
			if (is_dead) {
				Game::instance().changeScene(MENU, Game::instance().getPlayer());
			}
			else if (!Game::instance().getKey(KEY_J) && !Game::instance().getKey(KEY_K) && !Game::instance().getKey(KEY_L)) {
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
					orientation = LEFT;
				}
				else if (Game::instance().getKey(KEY_D))
				{
					if (sprite->animation() != MAIN_MR)
						sprite->changeAnimation(MAIN_MR);
					posPlayer.x += 2;
					if (map->collisionMoveRight(posPlayer, glm::ivec2(width_player, height_player)) || posPlayer.x + 90 >= max_x)
					{
						posPlayer.x -= 2;
						sprite->changeAnimation(MAIN_SR);
					}
					orientation = RIGHT;
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
					if (sprite->animation() == MAIN_ML || sprite->animation() == MAIN_KL || sprite->animation() == MAIN_PL || sprite->animation() == MAIN_SPL || sprite->animation() == MAIN_HL)
						sprite->changeAnimation(MAIN_SL);
					else if (sprite->animation() == MAIN_MR || sprite->animation() == MAIN_KR || sprite->animation() == MAIN_PR || sprite->animation() == MAIN_SPR || sprite->animation() == MAIN_HR)
						sprite->changeAnimation(MAIN_SR);
				}
			}
			if (Game::instance().getKey(KEY_K)) {
				Game::instance().keyReleased(KEY_K);
				if (sprite->animation() == MAIN_SL || sprite->animation() == MAIN_ML)//Abadede 35 a 110 colision ; 50 a 75
				{
					sprite->changeAnimation(MAIN_KL);
					checkCollisions('k');
				}
				else if (sprite->animation() == MAIN_SR || sprite->animation() == MAIN_MR)
				{
					sprite->changeAnimation(MAIN_KR);
						checkCollisions('k');
				}
			}
			else if (Game::instance().getKey(KEY_J)) {
				Game::instance().keyReleased(KEY_J);
				if (sprite->animation() == MAIN_SL || sprite->animation() == MAIN_ML)
				{
					sprite->changeAnimation(MAIN_PL);
					checkCollisions('j');
				}
				else if (sprite->animation() == MAIN_SR || sprite->animation() == MAIN_MR)
				{
					sprite->changeAnimation(MAIN_PR);
					checkCollisions('j');
				}
			}
			else if (Game::instance().getKey(KEY_L)) {
				Game::instance().keyReleased(KEY_L);
				if (sprite->animation() == MAIN_SL || sprite->animation() == MAIN_ML)
				{
					if (this->charType == HONDAT || this->charType == BISONT)
					posPlayer.x -= 70;
					if (map->collisionMoveLeft(posPlayer, glm::ivec2(width_player, height_player)) || posPlayer.x <= min_x)
						posPlayer.x += 70;
					sprite->changeAnimation(MAIN_SPL);
					checkCollisions('l');
				}
				else if (sprite->animation() == MAIN_SR || sprite->animation() == MAIN_MR)
				{
					if(this->charType==HONDAT || this->charType == BISONT)
					posPlayer.x += 70;
					if (map->collisionMoveRight(posPlayer, glm::ivec2(width_player, height_player)) || posPlayer.x > max_x)
						posPlayer.x -= 70;
					sprite->changeAnimation(MAIN_SPR);
					checkCollisions('l');
				}
			}
		}
	}
	else if (type_player == IA_PLAYER) {
		
		if (stateEnemy == DEAD && !one_time) 
		{
			one_time = true;
			if(orientation == RIGHT)
				sprite->changeAnimation(ENE_DL);
			else
				sprite->changeAnimation(ENE_DR);
		}

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

		if (sprite->getAnimationFinished() && stateEnemy != DEAD) {
			if (orientation == RIGHT && (stateEnemy == MOVING || stateEnemy == MOVING_TO_FIGHT))
				sprite->changeAnimation(ENE_MR);
			else if (orientation == LEFT && (stateEnemy == MOVING || stateEnemy == MOVING_TO_FIGHT))
				sprite->changeAnimation(ENE_ML);
			else if (orientation == RIGHT && stateEnemy == WAITING)
				sprite->changeAnimation(ENE_SR);
			else if (orientation == LEFT && stateEnemy == WAITING)
				sprite->changeAnimation(ENE_SL);
			else if (orientation == LEFT && stateEnemy == FIGHTING) {
				sprite->changeAnimation(ENE_PL);
				stateEnemy = WAITING;
				is_moving = false;
				checkCollisionsPlayer();
			}
			else if (orientation == RIGHT && stateEnemy == FIGHTING) {
				sprite->changeAnimation(ENE_PR);
				stateEnemy = WAITING;
				is_moving = false;
				checkCollisionsPlayer();
			}
		}
	
		if (orientation == RIGHT && stateEnemy == HITTED)
		{
			sprite->changeAnimation(ENE_HR);
			stateEnemy = WAITING;
			is_moving = false;
		}
		else if (orientation == LEFT && stateEnemy == HITTED)
		{
			sprite->changeAnimation(ENE_HL);
			stateEnemy = WAITING;
			is_moving = false;
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

	if ( ((posPlayer.x <= mainPlayer->getPosition().x + 90)&&(posPlayer.x >= mainPlayer->getPosition().x + 30)) || 
		 ((posPlayer.x + 120 >= mainPlayer->getPosition().x - 90)&& (posPlayer.x + 120 <= mainPlayer->getPosition().x - 30)) ) {
		if (posPlayer.y< mainPlayer->getPosition().y + 10 && posPlayer.y > mainPlayer->getPosition().y - 10) {
			std::random_device rd;
			std::uniform_int_distribution<int> dis(0, 4);
			int value = dis(rd);
			if (value == 0) {
				stateEnemy = FIGHTING;
				return;
			}
		}
	}

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
	
	if (stateEnemy == DEAD) return;

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

void Player::setHitted(int damage)
{
	if (type_player == USER_PLAYER) {

		hp -= damage;
		
		if (hp <= 0) {

			is_dead = true;
		}
		else {
			
			is_hitted = true;
		}
	}
	else {
		hp -= damage;
		if (hp <= 0 && stateEnemy != DEAD) {
			mainPlayer->setPoints(100);
			mainPlayer->sumaMuerto();
			stateEnemy = DEAD;
			if (charType == B1)
			{
				Game::instance().changeScene(LEVEL_2, Game::instance().getPlayer());
			}
		}
		else {
			mainPlayer->setPoints(10);
			stateEnemy = HITTED;
			timeHitted = 0;
			positionToMove = glm::ivec2(0, 0);
		}
	}
}

void Player::checkCollisions(char c)
{//left 0 right 1
	int minx1, miny1, maxx1, maxy1, minx2, miny2, maxx2, maxy2;
	int damage;
	bool diferentHit = false;
	if (this->charType == RYUT) {
		if (c == 'j') {
			damage = 8;
			if (this->orientation == RIGHT) {
				minx1 = posPlayer.x + 70; miny1 = posPlayer.y + 45;
				maxx1 = posPlayer.x + 120; maxy1 = posPlayer.y + 55;
			}
			else {
				maxx1 = posPlayer.x + (120-70); maxy1 = posPlayer.y + 55;
				minx1 = posPlayer.x + (120-120); miny1 = posPlayer.y + 45;
			}
		}
		else if (c == 'k') {
			damage = 12;
			if (this->orientation == RIGHT) {
				minx1 = posPlayer.x + 74; miny1 = posPlayer.y + 30;
				maxx1 = posPlayer.x + 105; maxy1 = posPlayer.y + 40;
			}
			else {
				maxx1 = posPlayer.x + (120 - 74); maxy1 = posPlayer.y + 40;
				minx1 = posPlayer.x + (120 - 105); miny1 = posPlayer.y + 30;
			}
		}
		else if (c == 'l') {
			damage = 20;
			if (this->orientation == RIGHT) {
				minx1 = posPlayer.x + 89; miny1 = posPlayer.y + 48;
				maxx1 = posPlayer.x + 135; maxy1 = posPlayer.y + 78;
			}
			else {
				maxx1 = posPlayer.x + (120 - 89); maxy1 = posPlayer.y + 78;
				minx1 = posPlayer.x + (120 - 135); miny1 = posPlayer.y + 48;
			}
		}
	}
	else if (this->charType == HONDAT) {
		if (c == 'j') {
			damage = 4;
			if (this->orientation == RIGHT) {
				minx1 = posPlayer.x + 80; miny1 = posPlayer.y + 50;
				maxx1 = posPlayer.x + 135; maxy1 = posPlayer.y + 75;
			}
			else {
				maxx1 = posPlayer.x + (120 - 80); maxy1 = posPlayer.y + 75;
				minx1 = posPlayer.x + (120 - 135); miny1 = posPlayer.y + 50;
			}
		}
		else if (c == 'k') {
			damage = 10;
			if (this->orientation == RIGHT) {
				minx1 = posPlayer.x + 80; miny1 = posPlayer.y + 90;
				maxx1 = posPlayer.x + 135; maxy1 = posPlayer.y + 110;
			}
			else {
				maxx1 = posPlayer.x + (120 - 80); maxy1 = posPlayer.y + 100;
				minx1 = posPlayer.x + (120 - 135); miny1 = posPlayer.y + 90;
			}
			diferentHit = true;
		}
		else if (c == 'l') {
			damage = 15;
			if (this->orientation == RIGHT) {
				minx1 = posPlayer.x + 20; miny1 = posPlayer.y + 15;
				maxx1 = posPlayer.x + 100; maxy1 = posPlayer.y + 75;
			}
			else {
				maxx1 = posPlayer.x + (120 - 20); maxy1 = posPlayer.y + 75;
				minx1 = posPlayer.x + (120 - 100); miny1 = posPlayer.y + 15;
			}
		}
	}
	if (this->charType == BISONT) {
		if (c == 'j') {
			damage = 9;
			if (this->orientation == RIGHT) {
				minx1 = posPlayer.x + 65; miny1 = posPlayer.y + 40;
				maxx1 = posPlayer.x + 135; maxy1 = posPlayer.y + 55;
			}
			else {
				maxx1 = posPlayer.x + (120 - 65); maxy1 = posPlayer.y + 55;
				minx1 = posPlayer.x + (120 - 135); miny1 = posPlayer.y + 40;
			}
		}
		else if (c == 'k') {
			damage = 5;
			if (this->orientation == RIGHT) {
				minx1 = posPlayer.x + 47; miny1 = posPlayer.y + 50;
				maxx1 = posPlayer.x + 125; maxy1 = posPlayer.y + 65;
			}
			else {
				maxx1 = posPlayer.x + (120 - 47); maxy1 = posPlayer.y + 65;
				minx1 = posPlayer.x + (120 - 125); miny1 = posPlayer.y + 50;
			}
		}
		else if (c == 'l') {
			damage = 25;
			if (this->orientation == RIGHT) {
				minx1 = posPlayer.x + 10; miny1 = posPlayer.y + 30;
				maxx1 = posPlayer.x + 125; maxy1 = posPlayer.y + 50;
			}
			else {
				maxx1 = posPlayer.x + (120 - 89); maxy1 = posPlayer.y + 50;
				minx1 = posPlayer.x + (120 - 135); miny1 = posPlayer.y + 30	;
			}
		}
	}
	
	for each(Player *p in enemies)
	{
		//Abadede 35 a 110 colision ; 50 a 75
		//pu�o (70 , 47) ; (95 , 53) 
		// (minx1 < maxx2) && (minx2 < maxx1)
		// (miny1 < maxy2) && (miny2 < maxy1)
		if (diferentHit == false) {
			if (p->charType == A) {
				if (p->orientation == RIGHT) {
					minx2 = p->getPosition().x + 40; miny2 = p->getPosition().y + 40;
					maxx2 = p->getPosition().x + 70; maxy2 = p->getPosition().y + 65;
				}
				else {
					maxx2 = p->getPosition().x + (120 - 40); maxy2 = p->getPosition().y + 65;
					minx2 = p->getPosition().x + (120 - 70); miny2 = p->getPosition().y + 40;
				}
			}
			else if (p->charType == J) {
				if (p->orientation == RIGHT) {
					minx2 = p->getPosition().x + 50; miny2 = p->getPosition().y + 50;
					maxx2 = p->getPosition().x + 80; maxy2 = p->getPosition().y + 80;
				}
				else {
					maxx2 = p->getPosition().x + (120 - 50); maxy2 = p->getPosition().y + 80;
					minx2 = p->getPosition().x + (120 - 80); miny2 = p->getPosition().y + 50;
				}
			}
			else if (p->charType == Z) {
				if (p->orientation == RIGHT) {
					minx2 = p->getPosition().x + 50; miny2 = p->getPosition().y + 60;
					maxx2 = p->getPosition().x + 80; maxy2 = p->getPosition().y + 75;
				}
				else {
					maxx2 = p->getPosition().x + (120 - 50); maxy2 = p->getPosition().y + 75;
					minx2 = p->getPosition().x + (120 - 80); miny2 = p->getPosition().y + 60;
				}
			}
			else if (p->charType == B1 || p->charType == B2) {
				if (p->orientation == RIGHT) {
					minx2 = p->getPosition().x + 40; miny2 = p->getPosition().y + 40;
					maxx2 = p->getPosition().x + 70; maxy2 = p->getPosition().y + 65;
				}
				else {
					maxx2 = p->getPosition().x + (120 - 40); maxy2 = p->getPosition().y + 65;
					minx2 = p->getPosition().x + (120 - 70); miny2 = p->getPosition().y + 40;
				}
			}
		}
		else {
			maxy2 = p->getPosition().y + 120;
			miny2 = p->getPosition().y + 100;
			if (p->charType == A) {
				if (p->orientation == LEFT) {
					minx2 = p->getPosition().x + 40;
					maxx2 = p->getPosition().x + 70;

				}
				else {
					minx2 = p->getPosition().x + (120 - 70);
					maxx2 = p->getPosition().x + (120 - 40);
				}
			}
		}
		if ((minx1 < maxx2) && (minx2<maxx1) && ((miny1 < maxy2 )&&(miny2<maxy1)))
		{
			p->setHitted(damage);
		}
	}
}

void Player::setEnemies(const vector<Player *> &enemies)
{
	this->enemies.clear();
	for each(Player *p in enemies) {
		this->enemies.push_back(p);
	}

}

void Player::checkCollisionsPlayer()
{
	int minx1, miny1, maxx1, maxy1, minx2, miny2, maxx2, maxy2;
	int damage;

	if (this->charType == A) {
		damage = 5;
		if (this->orientation == RIGHT) {
			minx1 = posPlayer.x + 90; miny1 = posPlayer.y + 50;
			maxx1 = posPlayer.x + 135; maxy1 = posPlayer.y + 65;
		}
		else {
			maxx1 = posPlayer.x + (120 - 90); maxy1 = posPlayer.y + 65;
			minx1 = posPlayer.x + (120 - 135); miny1 = posPlayer.y + 50;
		}
	}
	else if (this->charType == J) {
		damage = 7;
		if (this->orientation == RIGHT) {
			minx1 = posPlayer.x + 85; miny1 = posPlayer.y + 40;
			maxx1 = posPlayer.x + 135; maxy1 = posPlayer.y + 60;
		}
		else {
			maxx1 = posPlayer.x + (120 - 85); maxy1 = posPlayer.y + 60;
			minx1 = posPlayer.x + (120 - 135); miny1 = posPlayer.y + 40;
		}
	}
	else if (this->charType == Z) {
		damage = 10;
		if (this->orientation == RIGHT) {
			minx1 = posPlayer.x + 100; miny1 = posPlayer.y + 55;
			maxx1 = posPlayer.x + 140; maxy1 = posPlayer.y + 85;
		}
		else {
			maxx1 = posPlayer.x + (120 - 100); maxy1 = posPlayer.y + 85;
			minx1 = posPlayer.x + (120 - 140); miny1 = posPlayer.y + 55;
		}
	}
	else if (this->charType == B1 || this->charType == B2) {
		damage = 15;
		if (this->orientation == RIGHT) {
			minx1 = posPlayer.x + 90; miny1 = posPlayer.y + 50;
			maxx1 = posPlayer.x + 135; maxy1 = posPlayer.y + 65;
		}
		else {
			maxx1 = posPlayer.x + (120 - 90); maxy1 = posPlayer.y + 65;
			minx1 = posPlayer.x + (120 - 135); miny1 = posPlayer.y + 50;
		}
	}

	if (mainPlayer->charType == RYUT) {
		if (mainPlayer->orientation == RIGHT) {
			minx2 = mainPlayer->getPosition().x + 50; miny2 = mainPlayer->getPosition().y + 50;
			maxx2 = mainPlayer->getPosition().x + 70; maxy2 = mainPlayer->getPosition().y + 80;
		}
		else {
			maxx2 = mainPlayer->getPosition().x + (120 - 50); maxy2 = mainPlayer->getPosition().y + 80;
			minx2 = mainPlayer->getPosition().x + (120 - 70); miny2 = mainPlayer->getPosition().y + 50;
		}
	}
	else if (mainPlayer->charType == HONDAT) {
		if (mainPlayer->orientation == RIGHT) {
			minx2 = mainPlayer->getPosition().x + 40; miny2 = mainPlayer->getPosition().y + 50;
			maxx2 = mainPlayer->getPosition().x + 70; maxy2 = mainPlayer->getPosition().y + 80;
		}
		else {
			maxx2 = mainPlayer->getPosition().x + (120 - 40); maxy2 = mainPlayer->getPosition().y + 80;
			minx2 = mainPlayer->getPosition().x + (120 - 70); miny2 = mainPlayer->getPosition().y + 50;
		}
	}
	else if (mainPlayer->charType == BISONT) {
		if (mainPlayer->orientation == RIGHT) {
			minx2 = mainPlayer->getPosition().x + 40; miny2 = mainPlayer->getPosition().y + 40;
			maxx2 = mainPlayer->getPosition().x + 65; maxy2 = mainPlayer->getPosition().y + 70;
		}
		else {
			maxx2 = mainPlayer->getPosition().x + (120 - 40); maxy2 = mainPlayer->getPosition().y + 70;
			minx2 = mainPlayer->getPosition().x + (120 - 65); miny2 = mainPlayer->getPosition().y + 40;
		}
	}

	if ((minx1 < maxx2) && (minx2<maxx1) && ((miny1 < maxy2) && (miny2<maxy1)))
	{
		mainPlayer->setHitted(damage);
	}

}

int Player::getHP() 
{
	return hp;
}

void Player::setHP(int value)
{
	hp = value;
}

int Player::getPoints()
{
	return points;
}

void Player::setPoints(int value)
{
	points += value;
}

int Player::getState()
{
	return stateEnemy;
}

void Player::sumaMuerto()
{
	muertos++;
}

int Player::getMuertos()
{
	return muertos;
}

void Player::setState(int state)
{
	stateEnemy = state;
}

void Player::setMuertos(int n)
{
	muertos = n;
}