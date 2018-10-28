#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Level.h"

#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 250

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 547

enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, KICK_RIGHT, KICK_LEFT, PUNCH_RIGHT, PUNCH_LEFT, HADOUKEN_RIGHT, HADOUKEN_LEFT
};

Level::Level(int left, int right, int bottom, int top) : Scene(left, right, bottom, top)
{
	fullMap.scenario = NULL;
	fullMap.collision = NULL;
	mainPlayer = NULL;
}

Level::Level()
{
	fullMap.scenario = NULL;
	fullMap.collision = NULL;
	mainPlayer = NULL;
}


Level::~Level()
{
	if (fullMap.scenario != NULL)
		delete fullMap.scenario;
	if (fullMap.collision != NULL)
		delete fullMap.collision;
	if (mainPlayer != NULL)
		delete mainPlayer;
}

void Level::init()
{
	initShaders();
	fullMap.scenario = TileMap::createTileMap("levels/level01_object.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	fullMap.collision = TileMap::createTileMap("levels/level01_collision.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	setBackground("Resources/LevelBackground/Level_Bridge/bridge.png");
	//Main player
	mainPlayer = new Player();

	vector<pair<int, vector<glm::vec2>>> animations;
	animations.resize(10);
	animations[0] = make_pair(STAND_LEFT,  vector < glm::vec2> {	glm::vec2(0.55f, 0.5f), glm::vec2(0.50f, 0.5f),
																	glm::vec2(0.45f, 0.5f), glm::vec2(0.40f, 0.5f)}); //STAND_LEFT

	animations[1] = make_pair(STAND_RIGHT, vector < glm::vec2> {	glm::vec2(0.40f, 0.0f), glm::vec2(0.45f, 0.0f), 
																	glm::vec2(0.50f, 0.0f), glm::vec2(0.55f, 0.0f)}); //STAND_RIGHT
	
	animations[2] = make_pair(MOVE_LEFT,   vector < glm::vec2> {	glm::vec2(0.95f, 0.5f), glm::vec2(0.90f, 0.5f), 
																	glm::vec2(0.85f, 0.5f),  glm::vec2(0.80f, 0.5f), 
																	glm::vec2(0.75f, 0.5f)}); //MOVE_LEFT
	
	animations[3] = make_pair(MOVE_RIGHT,	vector < glm::vec2> {	glm::vec2(0.00f, 0.0f), glm::vec2(0.05f, 0.0f), 
																	glm::vec2(0.10f, 0.0f), glm::vec2(0.15f, 0.0f),
																	glm::vec2(0.20f, 0.0f)}); //MOVE_RIGHT
	
	animations[4] = make_pair(KICK_RIGHT,   vector < glm::vec2> {	glm::vec2(0.60f, 0.0f), glm::vec2(0.65f, 0.0f),
																	glm::vec2(0.70f, 0.0f)}); //KICK_RIGHT
	
	animations[5] = make_pair(KICK_LEFT,	vector < glm::vec2> {	glm::vec2(0.35f, 0.5f), glm::vec2(0.30f, 0.5f), 
																	glm::vec2(0.25f, 0.5f)}); //KICK_LEFT
	
	animations[6] = make_pair(PUNCH_RIGHT,  vector < glm::vec2> {	glm::vec2(0.25f, 0.0f), glm::vec2(0.30f, 0.0f), 
																	glm::vec2(0.35f, 0.0f)}); //PUNCH_RIGHT
	
	animations[7] = make_pair(PUNCH_LEFT,	vector < glm::vec2> {	glm::vec2(0.60f, 0.5f), glm::vec2(0.65f, 0.5f), 
																	glm::vec2(0.70f, 0.5f)}); //PUNCH_LEFT
	
	animations[8] = make_pair(HADOUKEN_RIGHT, vector < glm::vec2> {	glm::vec2(0.75f, 0.0f), glm::vec2(0.80f, 0.0f), 
																	glm::vec2(0.85f, 0.0f), glm::vec2(0.90f, 0.0f), 
																	glm::vec2(0.95f, 0.0f)}); //HADOUKEN_RIGHT

	animations[9] = make_pair(HADOUKEN_LEFT, vector < glm::vec2> {	glm::vec2(0.20f, 0.5f), glm::vec2(0.15f, 0.5f),
																	glm::vec2(0.10f, 0.5f), glm::vec2(0.05f, 0.5f),
																	glm::vec2(0.00f, 0.5f)}); //HADOUKEN_RIGHT
	
	mainPlayer->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "Resources/Sprites/ryu.png", animations);
	mainPlayer->setPosition(glm::vec2(INIT_PLAYER_X_TILES, INIT_PLAYER_Y_TILES));
	mainPlayer->setTileMap(fullMap.collision);//channge for scenario when collision load is diseabled

	projection = glm::ortho(float(cameraLeft), float(cameraRight), float(cameraBottom), float(cameraTop));
	currentTime = 0.0f;

	binit = true;
}

void Level::render()
{
	Scene::render();
	background->render(tex);
	fullMap.scenario->render();
	if (showCollisions) fullMap.collision->render();
	mainPlayer->render();
}

void Level::update(int deltaTime) {
	Scene::update(deltaTime);
	mainPlayer->update(deltaTime);
}

void Level::enableCollisionView(bool state) {
	showCollisions = state;
}

bool Level::getCollisionView() {
	return showCollisions;
}
void Level::moveCamera(int left, int right, int bottom, int top) {
	if (left >= 0 && right <= fullMap.scenario->getTileSize()) {
		this->cameraLeft = left;
		this->cameraRight = right;
		this->cameraBottom = bottom;
		this->cameraTop = top;
		projection = glm::ortho(float(left), float(right), float(bottom), float(top));
	}
}

bool Level::setBackground(const string &filename)
{
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(2080.f, 547.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	tex.loadFromFile(filename, TEXTURE_PIXEL_FORMAT_RGBA);

	return true;
}

bool Level::getInit()
{
	return binit;
}

void Level::restartLevel() 
{ 
	mainPlayer->setPosition(glm::vec2(INIT_PLAYER_X_TILES, INIT_PLAYER_Y_TILES)); 
	setCamera(glm::ivec4(0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, 0));
	projection = glm::ortho(float(cameraLeft), float(cameraRight), float(cameraBottom), float(cameraTop));
}


void setrenderMenu() {}