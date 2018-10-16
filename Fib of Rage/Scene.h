#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


struct FullMap
{
	TileMap *scenario;
	TileMap *collision;
};

class Scene
{

public:
	Scene(int left, int right, int bottom, int top);
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();
	void enableCollisionView(bool state);
	bool getCollisionView();
	void moveCamera(int left, int right, int bottom, int top);

private:
	void initShaders();

private:

	FullMap fullMap;

	Player *player;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	bool showCollisions = 0;
public:
	int cameraLeft;
	int cameraRight;
	int cameraBottom;
	int cameraTop;
};


#endif // _SCENE_INCLUDE

