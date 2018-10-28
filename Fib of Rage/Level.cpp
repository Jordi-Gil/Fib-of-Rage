#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Level.h"

#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 250

#define RYU_WIDTH 90*2
#define RYU_HEIGHT 120*2
#define ABA_WIDTH 120*2
#define ABA_HEIGHT 120*2

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 547

#define USER_PLAYER 0
#define IA_PLAYER 1

enum RyuAnims
{
	RYU_SL, RYU_SR, RYU_ML, RYU_MR, RYU_KR, RYU_KL, RYU_PR, RYU_PL, RYU_HR, RYU_HL
};

enum AbadeleAnims 
{
	ABA_SL, ABA_SR, ABA_ML, ABA_MR, ABA_PR, ABA_PL
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
	vector<pair<int, vector<glm::vec2>>> animations;
	animations.resize(10);
	animations[0] = make_pair(RYU_SL, vector < glm::vec2> {	glm::vec2(0.55f, 0.5f), glm::vec2(0.50f, 0.5f),
															glm::vec2(0.45f, 0.5f), glm::vec2(0.40f, 0.5f)}); //STAND_LEFT

	animations[1] = make_pair(RYU_SR, vector < glm::vec2> {	glm::vec2(0.40f, 0.0f), glm::vec2(0.45f, 0.0f),
															glm::vec2(0.50f, 0.0f), glm::vec2(0.55f, 0.0f)}); //STAND_RIGHT
	
	animations[2] = make_pair(RYU_ML, vector < glm::vec2> {	glm::vec2(0.95f, 0.5f), glm::vec2(0.90f, 0.5f),
															glm::vec2(0.85f, 0.5f),  glm::vec2(0.80f, 0.5f), 
															glm::vec2(0.75f, 0.5f)}); //MOVE_LEFT
	
	animations[3] = make_pair(RYU_MR, vector < glm::vec2> {	glm::vec2(0.00f, 0.0f), glm::vec2(0.05f, 0.0f),
															glm::vec2(0.10f, 0.0f), glm::vec2(0.15f, 0.0f),
															glm::vec2(0.20f, 0.0f)}); //MOVE_RIGHT
	
	animations[4] = make_pair(RYU_KR, vector < glm::vec2> {	glm::vec2(0.60f, 0.0f), glm::vec2(0.65f, 0.0f),
															glm::vec2(0.70f, 0.0f)}); //KICK_RIGHT
	
	animations[5] = make_pair(RYU_KL, vector < glm::vec2> {	glm::vec2(0.35f, 0.5f), glm::vec2(0.30f, 0.5f),
															glm::vec2(0.25f, 0.5f)}); //KICK_LEFT
	
	animations[6] = make_pair(RYU_PR, vector < glm::vec2> {	glm::vec2(0.25f, 0.0f), glm::vec2(0.30f, 0.0f),
															glm::vec2(0.35f, 0.0f)}); //PUNCH_RIGHT
	
	animations[7] = make_pair(RYU_PL, vector < glm::vec2> {	glm::vec2(0.60f, 0.5f), glm::vec2(0.65f, 0.5f),
															glm::vec2(0.70f, 0.5f)}); //PUNCH_LEFT
	
	animations[8] = make_pair(RYU_HR, vector < glm::vec2> {	glm::vec2(0.75f, 0.0f), glm::vec2(0.80f, 0.0f),
															glm::vec2(0.85f, 0.0f), glm::vec2(0.90f, 0.0f), 
															glm::vec2(0.95f, 0.0f)}); //HADOUKEN_RIGHT

	animations[9] = make_pair(RYU_HL, vector < glm::vec2> {	glm::vec2(0.20f, 0.5f), glm::vec2(0.15f, 0.5f),
															glm::vec2(0.10f, 0.5f), glm::vec2(0.05f, 0.5f),
															glm::vec2(0.00f, 0.5f)}); //HADOUKEN_RIGHT
	
	mainPlayer = new Player();
	mainPlayer->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "Resources/Sprites/Ryu/ryu.png", animations, glm::ivec2(RYU_WIDTH, RYU_HEIGHT), glm::vec2(0.05f,0.5f), USER_PLAYER);
	mainPlayer->setPosition(glm::vec2(INIT_PLAYER_X_TILES, INIT_PLAYER_Y_TILES));
	mainPlayer->setTileMap(fullMap.collision);//channge for scenario when collision load is diseabled

	animations.clear();
	animations.resize(6);

	animations[0] = make_pair(ABA_SL, vector < glm::vec2> {	glm::vec2(0.00f, 0.0f), glm::vec2(1/12.f, 0.0f),
															glm::vec2(2/12.f, 0.0f)}); //STAND_LEFT

	animations[1] = make_pair(ABA_SR, vector < glm::vec2> {	glm::vec2(11/12.f, 0.5f), glm::vec2(10/12.f, 0.5f),
															glm::vec2(9/12.f, 0.5f)}); //STAND_RIGHT

	animations[2] = make_pair(ABA_ML, vector < glm::vec2> {	glm::vec2(3/12.f, 0.0f), glm::vec2(4/12.f, 0.0f),
															glm::vec2(5/12.f, 0.0f), glm::vec2(6/12.f, 0.0f),
															glm::vec2(7/12.f, 0.0f)}); //MOVE_LEFT

	animations[3] = make_pair(ABA_MR, vector < glm::vec2> {	glm::vec2(8/12.f, 0.5f), glm::vec2(7/12.f, 0.5f),
															glm::vec2(6/12.f, 0.5f), glm::vec2(5/12.f, 0.5f),
															glm::vec2(4/12.f, 0.5f)}); //MOVE_RIGHT

	animations[4] = make_pair(ABA_PR, vector < glm::vec2> {	glm::vec2(3/12.f, 0.5f), glm::vec2(2/12.f, 0.5f),
															glm::vec2(1/12.f, 0.5f), glm::vec2(0.000f, 0.5f)}); //PUNCH_RIGHT

	animations[5] = make_pair(ABA_PL, vector < glm::vec2> {	glm::vec2(8/12.f, 0.0f), glm::vec2(9/12.f, 0.5f),
															glm::vec2(10/12.f, 0.0f), glm::vec2(11/12.f, 0.0f)}); //PUNCH_LEFT

	enemy[0] = new Player();
	enemy[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "Resources/Sprites/Abadede/abadede_enemy_1.png", animations, glm::ivec2(ABA_WIDTH, ABA_HEIGHT), glm::vec2(1/12.f, 0.5f), IA_PLAYER);
	enemy[0]->setPosition(glm::vec2(INIT_PLAYER_X_TILES+300, INIT_PLAYER_Y_TILES));
	enemy[0]->setTileMap(fullMap.collision);//channge for scenario when collision load is diseabled

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
	if (mainPlayer->getPosition().y > enemy[0]->getPosition().y) {
		enemy[0]->render();
		mainPlayer->render();
	}
	else {
		mainPlayer->render();
		enemy[0]->render();
	}
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
