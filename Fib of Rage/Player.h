#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, const string &filename, vector<pair<int, vector<glm::vec2>>> &animations, glm::ivec2 tam, glm::vec2 prop, int type, Player *player);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	glm::ivec2 getPosition();
	void changeState();

	void set_X_max_min(int x_max, int x_min);

private:
	void move_player_to_fight();
	void move_around_player();
	void gotoDestination();
private:

	bool bJumping;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	Player *mainPlayer;


	int width_player, height_player;
	int stateEnemy;
	int timeHitted;
	bool direction; // 0 left 1 right

	int type_player; // 0 - User 1 - "IA"

	glm::ivec2 positionToMove;

	int max_x;
	int min_x;
	int max_y = 318;
	int min_y = 206;

	bool freeChooseDest = true;

};


#endif // _PLAYER_INCLUDE


