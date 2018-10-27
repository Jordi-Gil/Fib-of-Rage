#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include <SOIL.h>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Quad.h"
#include "TexturedQuad.h"


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
	void setCamera(glm::ivec4 pos);

	virtual bool getInit();

	virtual void update(int deltaTime);
	virtual void render();
	virtual void init();

	virtual void enableCollisionView(bool state);
	virtual bool getCollisionView();
	virtual void moveCamera(int left, int right, int bottom, int top);

	virtual void restartLevel();

protected:
	void initShaders();

protected:
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	bool binit = false;

public:
	int cameraLeft;
	int cameraRight;
	int cameraBottom;
	int cameraTop;

};


#endif // _SCENE_INCLUDE

