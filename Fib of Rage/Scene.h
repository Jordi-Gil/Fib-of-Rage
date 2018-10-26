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

	virtual void update(int deltaTime);
	virtual void render();
	virtual void init();

	virtual void enableCollisionView(bool state);
	virtual bool getCollisionView();
	virtual void moveCamera(int left, int right, int bottom, int top);

protected:
	void initShaders();

protected:
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;


public:
	int cameraLeft;
	int cameraRight;
	int cameraBottom;
	int cameraTop;

};


#endif // _SCENE_INCLUDE

