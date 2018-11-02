#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include<vector>
#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

enum CharType { A, J, Z, B1, B2, RYUT, HONDAT,BISONT};

class Player
{

public:
	void init(	const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, const string &filename, vector<pair<int, vector<glm::vec2>>> &animations, 
				glm::ivec2 tam, glm::vec2 prop, int type, Player *player,CharType enemyType);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	glm::ivec2 getPosition();
	void changeState();

	void set_X_max_min(int x_max, int x_min);
	void setHitted();
	void setEnemies(const vector<Player *> &enemies);
	CharType charType;

private:
	void move_player_to_fight();
	void move_around_player();
	void gotoDestination();
	void checkCollisions(char c);

private:

	bool bJumping;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	Player *mainPlayer;
	vector<Player*> enemies;

	int width_player, height_player;
	int stateEnemy;
	int timeHitted = 25;
	int orientation; // 0 left 1 right

	int type_player; // 0 - User 1 - "IA"

	glm::ivec2 positionToMove;

	int max_x;
	int min_x;
	const int max_y = 318;
	const int min_y = 206;
	bool is_moving = false;
	const int speed_player = 2;

	bool freeChooseDest = true;

};


#endif // _PLAYER_INCLUDE


