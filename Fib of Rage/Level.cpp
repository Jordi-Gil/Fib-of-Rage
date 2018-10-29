#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <algorithm>
#include "Level.h"

#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 250

#define P_WIDTH 90*2
#define P_HEIGHT 120*2

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 547

#define USER_PLAYER 0
#define IA_PLAYER 1

enum RyuAnims
{
	RYU_SL, RYU_SR, RYU_ML, RYU_MR, RYU_KR, RYU_KL, RYU_PR, RYU_PL, RYU_HR, RYU_HL
};

enum BisonAnims
{
	BIS_SL, BIS_SR, BIS_ML, BIS_MR, BIS_KR, BIS_KL, BIS_PR, BIS_PL, BIS_SPR, BIS_SPL
};

enum HondaAnims
{
	HON_SL, HON_SR, HON_ML, HON_MR, HON_KR, HON_KL, HON_PR, HON_PL, HON_SPR, HON_SPL
};

enum AbadeleAnims 
{
	ABA_SL, ABA_SR, ABA_ML, ABA_MR, ABA_PR, ABA_PL
};

enum JackAnims
{
	JAK_SL, JAK_SR, JAK_MR, JAK_ML, JAK_PR, JAK_PL
};

enum ZamzaAnims
{
	ZAM_SL, ZAM_SR, ZAM_MR, ZAM_ML, ZAM_PR, ZAM_PL
};

bool compare(Player *a, Player *b)
{
	return (a->getPosition().y > b->getPosition().y);
}

Level::Level(int left, int right, int bottom, int top) : Scene(left, right, bottom, top)
{
	collision = NULL;
	mainPlayer = NULL;
	characters.resize(5);
	for each(Player *player in characters) player = NULL;
}

Level::Level()
{
	collision = NULL;
	characters.resize(6);
	for each(Player *player in characters) player = NULL;
}

Level::~Level()
{
	if (collision != NULL)
		delete collision;
	if (mainPlayer != NULL)
		delete mainPlayer;
	for each(Player *player in characters)
		if (player != NULL)
			delete player;
}

void Level::init()
{
	initShaders();
	collision = TileMap::createTileMap("levels/level01_collision.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	setBackground("Resources/LevelBackground/Level_Bridge/bridge.png");
	setAnimations();

	projection = glm::ortho(float(cameraLeft), float(cameraRight), float(cameraBottom), float(cameraTop));
	currentTime = 0.0f;

	binit = true;
}

void Level::render()
{
	Scene::render();
	background->render(tex);
	if (showCollisions) collision->render();
	sort(characters.begin(), characters.end(), compare);
	bool rendered = false;
	for each (Player *player in characters)
	{
		if (mainPlayer->getPosition().y > player->getPosition().y) {
			mainPlayer->render();
			rendered = true;
		}
		player->render();
	}
	
	if (!rendered) mainPlayer->render();
}

void Level::update(int deltaTime) {
	Scene::update(deltaTime);
	mainPlayer->update(deltaTime);
	for each(Player *player in characters) player->update(deltaTime);
}

void Level::enableCollisionView(bool state) {
	showCollisions = state;
}

bool Level::getCollisionView() {
	return showCollisions;
}

void Level::moveCamera(int left, int right, int bottom, int top) {
	if (left >= 0 && right <= collision->getTileSize()) {
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
	int offsetX = 100, offsetY = 10;
	for each(Player *player in characters) {
		player->setPosition(glm::vec2(INIT_PLAYER_X_TILES + offsetX, INIT_PLAYER_Y_TILES + offsetY));
		offsetX += 100; offsetY += 10;
	}
	
	setCamera(glm::ivec4(0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, 0));
	projection = glm::ortho(float(cameraLeft), float(cameraRight), float(cameraBottom), float(cameraTop));
}

void Level::setAnimations()
{
	vector<pair<int, vector<glm::vec2>>> animations;
	//Ryu
	animations.resize(10);

	animations[0] = make_pair(RYU_SL, vector < glm::vec2> {	glm::vec2(0.55f, 0.5f), glm::vec2(0.50f, 0.5f), glm::vec2(0.45f, 0.5f), glm::vec2(0.40f, 0.5f)}); //STAND_LEFT
	animations[1] = make_pair(RYU_SR, vector < glm::vec2> {	glm::vec2(0.40f, 0.0f), glm::vec2(0.45f, 0.0f), glm::vec2(0.50f, 0.0f), glm::vec2(0.55f, 0.0f)}); //STAND_RIGHT
	animations[2] = make_pair(RYU_ML, vector < glm::vec2> {	glm::vec2(0.95f, 0.5f), glm::vec2(0.90f, 0.5f), glm::vec2(0.85f, 0.5f), glm::vec2(0.80f, 0.5f), glm::vec2(0.75f, 0.5f)}); //MOVE_LEFT
	animations[3] = make_pair(RYU_MR, vector < glm::vec2> {	glm::vec2(0.00f, 0.0f), glm::vec2(0.05f, 0.0f), glm::vec2(0.10f, 0.0f), glm::vec2(0.15f, 0.0f), glm::vec2(0.20f, 0.0f)}); //MOVE_RIGHT
	animations[4] = make_pair(RYU_KR, vector < glm::vec2> {	glm::vec2(0.60f, 0.0f), glm::vec2(0.65f, 0.0f), glm::vec2(0.70f, 0.0f)}); //KICK_RIGHT
	animations[5] = make_pair(RYU_KL, vector < glm::vec2> {	glm::vec2(0.35f, 0.5f), glm::vec2(0.30f, 0.5f), glm::vec2(0.25f, 0.5f)}); //KICK_LEFT
	animations[6] = make_pair(RYU_PR, vector < glm::vec2> {	glm::vec2(0.25f, 0.0f), glm::vec2(0.30f, 0.0f), glm::vec2(0.35f, 0.0f)}); //PUNCH_RIGHT
	animations[7] = make_pair(RYU_PL, vector < glm::vec2> {	glm::vec2(0.60f, 0.5f), glm::vec2(0.65f, 0.5f), glm::vec2(0.70f, 0.5f)}); //PUNCH_LEFT
	animations[8] = make_pair(RYU_HR, vector < glm::vec2> {	glm::vec2(0.75f, 0.0f), glm::vec2(0.80f, 0.0f), glm::vec2(0.85f, 0.0f), glm::vec2(0.90f, 0.0f), glm::vec2(0.95f, 0.0f)}); //HADOUKEN_RIGHT
	animations[9] = make_pair(RYU_HL, vector < glm::vec2> {	glm::vec2(0.20f, 0.5f), glm::vec2(0.15f, 0.5f), glm::vec2(0.10f, 0.5f), glm::vec2(0.05f, 0.5f), glm::vec2(0.00f, 0.5f)}); //HADOUKEN_RIGHT

	mainPlayer = new Player();
	mainPlayer->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "Resources/Sprites/Ryu/ryu.png", animations, glm::ivec2(P_WIDTH, P_HEIGHT), glm::vec2(0.05f, 0.5f), USER_PLAYER, NULL);
	mainPlayer->setPosition(glm::vec2(INIT_PLAYER_X_TILES, INIT_PLAYER_Y_TILES));
	mainPlayer->setTileMap(collision); //channge for scenario when collision load is diseabled
	/*
	characters[0] = new Player();
	characters[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "Resources/Sprites/Ryu/ryu.png", animations, glm::ivec2(P_WIDTH, P_HEIGHT), glm::vec2(0.05f, 0.5f), USER_PLAYER);
	characters[0]->setPosition(glm::vec2(INIT_PLAYER_X_TILES, INIT_PLAYER_Y_TILES));
	characters[0]->setTileMap(fullMap.collision); //channge for scenario when collision load is diseabled
	*/
	//Bison
	animations.clear();
	animations.resize(10);

	animations[0] = make_pair(BIS_SL, vector < glm::vec2> {	glm::vec2(22 / 23.f, 0.5f), glm::vec2(21 / 23.f, 0.5f), glm::vec2(21 / 23.f, 0.5f)}); //STAND_LEFT
	animations[1] = make_pair(BIS_SR, vector < glm::vec2> {	glm::vec2(0.00f, 0.0f), glm::vec2(1 / 23.f, 0.0f), glm::vec2(2 / 23.f, 0.0f)}); //STAND_RIGHT
	animations[2] = make_pair(BIS_ML, vector < glm::vec2> {	glm::vec2(19 / 23.f, 0.5f), glm::vec2(18 / 23.f, 0.5f), glm::vec2(17 / 23.f, 0.5f), glm::vec2(16 / 23.f, 0.5f)}); //MOVE_LEFT
	animations[3] = make_pair(BIS_MR, vector < glm::vec2> {	glm::vec2(3 / 23.f, 0.0f), glm::vec2(4 / 23.f, 0.0f), glm::vec2(5 / 23.f, 0.0f), glm::vec2(6 / 23.f, 0.0f)}); //MOVE_RIGHT
	animations[4] = make_pair(BIS_KR, vector < glm::vec2> {	glm::vec2(11 / 23.f, 0.0f), glm::vec2(12 / 23.f, 0.0f), glm::vec2(13 / 23.f, 0.0f), glm::vec2(14 / 23.f, 0.0f)}); //KICK_RIGHT
	animations[5] = make_pair(BIS_KL, vector < glm::vec2> {	glm::vec2(11 / 23.f, 0.5f), glm::vec2(10 / 23.f, 0.5f), glm::vec2(9 / 23.f, 0.5f), glm::vec2(8 / 23.f, 0.5f)}); //KICK_LEFT
	animations[6] = make_pair(BIS_PR, vector < glm::vec2> {	glm::vec2(7 / 23.f, 0.0f), glm::vec2(8 / 23.f, 0.0f), glm::vec2(9 / 23.f, 0.0f), glm::vec2(10 / 23.f, 0.0f)}); //PUNCH_RIGHT
	animations[7] = make_pair(BIS_PL, vector < glm::vec2> {	glm::vec2(15 / 23.f, 0.5f), glm::vec2(14 / 23.f, 0.5f), glm::vec2(13 / 23.f, 0.5f), glm::vec2(12 / 23.f, 0.5f)}); //PUNCH_LEFT
	animations[8] = make_pair(BIS_SPR, vector < glm::vec2> {	glm::vec2(15 / 23.f, 0.5f), glm::vec2(16 / 23.f, 0.0f), glm::vec2(17 / 23.f, 0.0f), glm::vec2(18 / 23.f, 0.0f), glm::vec2(19 / 23.f, 0.0f), glm::vec2(20 / 23.f, 0.0f), glm::vec2(21 / 23.f, 0.0f), glm::vec2(22 / 23.f, 0.0f)}); //SPECIAL_RIGHT
	animations[9] = make_pair(BIS_SPL, vector < glm::vec2> {	glm::vec2(7 / 23.f, 0.5f), glm::vec2(6 / 23.f, 0.5f), glm::vec2(5 / 23.f, 0.5f), glm::vec2(4 / 23.f, 0.5f), glm::vec2(3 / 23.f, 0.5f), glm::vec2(2 / 23.f, 0.0f), glm::vec2(1 / 23.f, 0.0f), glm::vec2(0.00f, 0.0f)}); //SPECIAL_RIGHT

	characters[0] = new Player();
	characters[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "Resources/Sprites/Bison/bison.png", animations, glm::ivec2(P_WIDTH, P_HEIGHT), glm::vec2(1 / 23.f, 0.5f), IA_PLAYER, mainPlayer);
	characters[0]->setPosition(glm::vec2(INIT_PLAYER_X_TILES + 300, INIT_PLAYER_Y_TILES));
	characters[0]->setTileMap(collision);//channge for scenario when collision load is diseabled

	//Honda
	animations.clear();
	animations.resize(10);

	animations[0] = make_pair(HON_SL, vector < glm::vec2> {	glm::vec2(20 / 21.f, 0.5f), glm::vec2(19 / 21.f, 0.5f), glm::vec2(18 / 21.f, 0.5f), glm::vec2(17 / 21.f, 0.5f)}); //STAND_LEFT
	animations[1] = make_pair(HON_SR, vector < glm::vec2> {	glm::vec2(0.00f, 0.0f), glm::vec2(1 / 21.f, 0.0f), glm::vec2(2 / 21.f, 0.0f), glm::vec2(3 / 21.f, 0.0f)}); //STAND_RIGHT
	animations[2] = make_pair(HON_ML, vector < glm::vec2> {	glm::vec2(16 / 21.f, 0.5f), glm::vec2(15 / 21.f, 0.5f), glm::vec2(14 / 21.f, 0.5f), glm::vec2(13 / 21.f, 0.5f)}); //MOVE_LEFT
	animations[3] = make_pair(HON_MR, vector < glm::vec2> {	glm::vec2(4 / 21.f, 0.0f), glm::vec2(5 / 21.f, 0.0f), glm::vec2(6 / 21.f, 0.0f), glm::vec2(7 / 21.f, 0.0f)}); //MOVE_RIGHT
	animations[4] = make_pair(HON_KR, vector < glm::vec2> {	glm::vec2(11 / 21.f, 0.0f), glm::vec2(12 / 21.f, 0.0f), glm::vec2(13 / 21.f, 0.0f)}); //KICK_RIGHT
	animations[5] = make_pair(HON_KL, vector < glm::vec2> {	glm::vec2(9 / 21.f, 0.5f), glm::vec2(8 / 21.f, 0.5f), glm::vec2(7 / 21.f, 0.5f)}); //KICK_LEFT
	animations[6] = make_pair(HON_PR, vector < glm::vec2> {	glm::vec2(8 / 21.f, 0.0f), glm::vec2(9 / 21.f, 0.0f), glm::vec2(10 / 21.f, 0.0f)}); //PUNCH_RIGHT
	animations[7] = make_pair(HON_PL, vector < glm::vec2> {	glm::vec2(12 / 21.f, 0.5f), glm::vec2(11 / 21.f, 0.5f), glm::vec2(10 / 21.f, 0.5f)}); //PUNCH_LEFT
	animations[8] = make_pair(HON_SPR, vector < glm::vec2> { glm::vec2(14 / 21.f, 0.5f), glm::vec2(15 / 21.f, 0.0f), glm::vec2(16 / 21.f, 0.0f), glm::vec2(17 / 21.f, 0.0f), glm::vec2(18 / 21.f, 0.0f), glm::vec2(19 / 21.f, 0.0f), glm::vec2(20 / 21.f, 0.0f)}); //SPECIAL_RIGHT
	animations[9] = make_pair(HON_SPL, vector < glm::vec2> { glm::vec2(6 / 21.f, 0.5f), glm::vec2(5 / 21.f, 0.5f), glm::vec2(4 / 21.f, 0.5f), glm::vec2(3 / 21.f, 0.5f), glm::vec2(2 / 21.f, 0.0f), glm::vec2(1 / 21.f, 0.0f), glm::vec2(0.00f, 0.0f)}); //SPECIAL_RIGHT

	characters[1] = new Player();
	characters[1]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "Resources/Sprites/Honda/honda.png", animations, glm::ivec2(P_WIDTH, P_HEIGHT), glm::vec2(1 / 21.f, 0.5f), IA_PLAYER, mainPlayer);
	characters[1]->setPosition(glm::vec2(INIT_PLAYER_X_TILES + 400, INIT_PLAYER_Y_TILES + 10));
	characters[1]->setTileMap(collision);//channge for scenario when collision load is diseabled

	//Abadede
	animations.clear();
	animations.resize(6);

	animations[0] = make_pair(ABA_SL, vector < glm::vec2> {	glm::vec2(0.00f, 0.0f), glm::vec2(1 / 12.f, 0.0f), glm::vec2(2 / 12.f, 0.0f)}); //STAND_LEFT
	animations[1] = make_pair(ABA_SR, vector < glm::vec2> {	glm::vec2(11 / 12.f, 0.5f), glm::vec2(10 / 12.f, 0.5f), glm::vec2(9 / 12.f, 0.5f)}); //STAND_RIGHT
	animations[2] = make_pair(ABA_ML, vector < glm::vec2> {	glm::vec2(3 / 12.f, 0.0f), glm::vec2(4 / 12.f, 0.0f), glm::vec2(5 / 12.f, 0.0f), glm::vec2(6 / 12.f, 0.0f), glm::vec2(7 / 12.f, 0.0f)}); //MOVE_LEFT
	animations[3] = make_pair(ABA_MR, vector < glm::vec2> {	glm::vec2(8 / 12.f, 0.5f), glm::vec2(7 / 12.f, 0.5f), glm::vec2(6 / 12.f, 0.5f), glm::vec2(5 / 12.f, 0.5f), glm::vec2(4 / 12.f, 0.5f)}); //MOVE_RIGHT
	animations[4] = make_pair(ABA_PR, vector < glm::vec2> {	glm::vec2(3 / 12.f, 0.5f), glm::vec2(2 / 12.f, 0.5f), glm::vec2(1 / 12.f, 0.5f), glm::vec2(0.000f, 0.5f)}); //PUNCH_RIGHT
	animations[5] = make_pair(ABA_PL, vector < glm::vec2> {	glm::vec2(8 / 12.f, 0.0f), glm::vec2(9 / 12.f, 0.0f), glm::vec2(10 / 12.f, 0.0f), glm::vec2(11 / 12.f, 0.0f)}); //PUNCH_LEFT

	characters[2] = new Player();
	characters[2]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "Resources/Sprites/Abadede/abadede_enemy_1.png", animations, glm::ivec2(P_WIDTH, P_HEIGHT), glm::vec2(1 / 12.f, 0.5f), IA_PLAYER, mainPlayer);
	characters[2]->setPosition(glm::vec2(INIT_PLAYER_X_TILES + 500, INIT_PLAYER_Y_TILES + 20));
	characters[2]->setTileMap(collision);//channge for scenario when collision load is diseabled

	//Jack
	animations.clear();
	animations.resize(6);

	animations[0] = make_pair(JAK_SL, vector < glm::vec2> {	glm::vec2(0.00f, 0.0f), glm::vec2(1 / 13.f, 0.0f), glm::vec2(2 / 13.f, 0.0f)}); //STAND_LEFT
	animations[1] = make_pair(JAK_SR, vector < glm::vec2> {	glm::vec2(12 / 13.f, 0.5f), glm::vec2(11 / 13.f, 0.5f), glm::vec2(10 / 13.f, 0.5f)}); //STAND_RIGHT
	animations[2] = make_pair(JAK_MR, vector < glm::vec2> {	glm::vec2(9 / 13.f, 0.5f), glm::vec2(8 / 13.f, 0.5f), glm::vec2(7 / 13.f, 0.5f), glm::vec2(6 / 13.f, 0.5f)}); //MOVE_LEFT
	animations[3] = make_pair(JAK_ML, vector < glm::vec2> {	glm::vec2(3 / 13.f, 0.0f), glm::vec2(4 / 13.f, 0.0f), glm::vec2(5 / 13.f, 0.0f), glm::vec2(6 / 13.f, 0.0f)}); //MOVE_RIGHT
	animations[4] = make_pair(JAK_PR, vector < glm::vec2> {	glm::vec2(5 / 13.f, 0.5f), glm::vec2(4 / 13.f, 0.5f), glm::vec2(3 / 13.f, 0.5f), glm::vec2(2 / 13.f, 0.5f), glm::vec2(1 / 13.f, 0.5f), glm::vec2(0.00f, 0.5f)}); //PUNCH_RIGHT
	animations[5] = make_pair(JAK_PL, vector < glm::vec2> {	glm::vec2(7 / 13.f, 0.0f), glm::vec2(8 / 13.f, 0.0f), glm::vec2(9 / 13.f, 0.0f), glm::vec2(10 / 13.f, 0.0f), glm::vec2(11 / 13.f, 0.0f), glm::vec2(12 / 13.f, 0.0f)}); //PUNCH_LEFT

	characters[3] = new Player();
	characters[3]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "Resources/Sprites/Jack/jack_enemy_3.png", animations, glm::ivec2(P_WIDTH, P_HEIGHT), glm::vec2(1 / 13.f, 0.5f), IA_PLAYER, mainPlayer);
	characters[3]->setPosition(glm::vec2(INIT_PLAYER_X_TILES + 600, INIT_PLAYER_Y_TILES + 30));
	characters[3]->setTileMap(collision);//channge for scenario when collision load is diseabled

	//Zamza
	animations.clear();
	animations.resize(6);

	animations[0] = make_pair(ZAM_SL, vector < glm::vec2> {	glm::vec2(0.00f, 0.0f), glm::vec2(1 / 12.f, 0.0f), glm::vec2(2 / 12.f, 0.0f)}); //STAND_LEFT
	animations[1] = make_pair(ZAM_SR, vector < glm::vec2> {	glm::vec2(11 / 12.f, 0.5f), glm::vec2(10 / 12.f, 0.5f), glm::vec2(9 / 12.f, 0.5f)}); //STAND_RIGHT
	animations[2] = make_pair(ZAM_MR, vector < glm::vec2> {	glm::vec2(3 / 12.f, 0.0f), glm::vec2(4 / 12.f, 0.0f), glm::vec2(5 / 12.f, 0.0f), glm::vec2(6 / 12.f, 0.0f), glm::vec2(7 / 12.f, 0.0f), glm::vec2(8 / 12.f, 0.0f)}); //MOVE_LEFT
	animations[3] = make_pair(ZAM_ML, vector < glm::vec2> {	glm::vec2(8 / 12.f, 0.5f), glm::vec2(7 / 12.f, 0.5f), glm::vec2(6 / 12.f, 0.5f), glm::vec2(5 / 12.f, 0.5f), glm::vec2(4 / 12.f, 0.5f), glm::vec2(3 / 12.f, 0.5f)}); //MOVE_RIGHT
	animations[4] = make_pair(ZAM_PR, vector < glm::vec2> {	glm::vec2(2 / 12.f, 0.5f), glm::vec2(1 / 12.f, 0.5f), glm::vec2(0.000f, 0.5f)}); //PUNCH_RIGHT
	animations[5] = make_pair(ZAM_PL, vector < glm::vec2> {	glm::vec2(9 / 12.f, 0.0f), glm::vec2(10 / 12.f, 0.0f), glm::vec2(11 / 12.f, 0.0f)}); //PUNCH_LEFT

	characters[4] = new Player();
	characters[4]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "Resources/Sprites/Zamza/zamza_enemy_2.png", animations, glm::ivec2(P_WIDTH, P_HEIGHT), glm::vec2(1 / 12.f, 0.5f), IA_PLAYER, mainPlayer);
	characters[4]->setPosition(glm::vec2(INIT_PLAYER_X_TILES + 700, INIT_PLAYER_Y_TILES + 80));
	characters[4]->setTileMap(collision);//channge for scenario when collision load is diseabled

}