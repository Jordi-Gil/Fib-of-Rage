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


enum PLAYERS 
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

bool compare(Player *a, Player *b)
{
	return (a->getPosition().y < b->getPosition().y);
}

Level::Level(int left, int right, int bottom, int top) : Scene(left, right, bottom, top)
{
	max_x = right;
	min_x = left;
	collision = NULL;
	mainPlayer = NULL;
	characters.resize(3);
	for each(Player *player in characters) player = NULL;
}

Level::Level()
{
	collision = NULL;
	characters.resize(3);
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

void Level::init(int player)
{
	initShaders();
	collision = TileMap::createTileMap("levels/level01_collision.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	setBackground("Resources/LevelBackground/Level_Bridge/bridge.png");
	userPlayer = player;
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
		if (!rendered && (mainPlayer->getPosition().y + P_HEIGHT) < (player->getPosition().y + P_HEIGHT)) {
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
	for each(Player *player in characters) {
		player->update(deltaTime);
		player->changeState();

	}
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

void Level::restartLevel(int player) 
{ 
	if (player != userPlayer) {
		userPlayer = player;
		setMainPlayer();
	}
	mainPlayer->setPosition(glm::vec2(INIT_PLAYER_X_TILES, INIT_PLAYER_Y_TILES)); 
	int offsetY = 10;
	for each(Player *player in characters) {
		player->setPosition(glm::vec2(1150, INIT_PLAYER_Y_TILES + offsetY));
		offsetY += 10;
	}
	
	setCamera(glm::ivec4(0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, 0));
	projection = glm::ortho(float(cameraLeft), float(cameraRight), float(cameraBottom), float(cameraTop));
}

void Level::setAnimations()
{
	setMainPlayer();
	vector<pair<int, vector<glm::vec2>>> animations;

	//Abadede
	animations.clear();
	animations.resize(10);

	animations[0] = make_pair(ENE_SR, vector < glm::vec2> {	glm::vec2(0.0f, 0.0f), glm::vec2(1 / 16.f, 0.0f), glm::vec2(2 / 16.f, 0.0f)}); //STAND RIGHT
	animations[1] = make_pair(ENE_SL, vector < glm::vec2> {	glm::vec2(15 / 16.f, 0.5f), glm::vec2(14 / 16.f, 0.5f), glm::vec2(13 / 16.f, 0.5f)}); //STAND LEFT
	animations[2] = make_pair(ENE_MR, vector < glm::vec2> {	glm::vec2(3  / 16.f, 0.0f), glm::vec2(4  / 16.f, 0.0f), glm::vec2(5  / 16.f, 0.0f), glm::vec2(6  / 16.f, 0.0f), glm::vec2(7  / 16.f, 0.0f)}); //MOVE RIGHT
	animations[3] = make_pair(ENE_ML, vector < glm::vec2> {	glm::vec2(12 / 16.f, 0.5f), glm::vec2(11 / 16.f, 0.5f), glm::vec2(10 / 16.f, 0.5f), glm::vec2(9  / 16.f, 0.5f), glm::vec2(8  / 16.f, 0.5f)}); //MOVE LEFT
	animations[4] = make_pair(ENE_PR, vector < glm::vec2> {	glm::vec2(8  / 16.f, 0.0f), glm::vec2(9  / 16.f, 0.0f), glm::vec2(10 / 16.f, 0.0f), glm::vec2(11 / 16.f, 0.0f)}); //PUNCH RIGHT
	animations[5] = make_pair(ENE_PL, vector < glm::vec2> {	glm::vec2(7  / 16.f, 0.5f), glm::vec2(6  / 16.f, 0.5f), glm::vec2(5  / 16.f, 0.5f), glm::vec2(4  / 16.f, 0.5f)}); //PUNCH LEFT
	animations[6] = make_pair(ENE_HR, vector < glm::vec2> { glm::vec2(13 / 16.f, 0.0f)}); //HIT RIGHT
	animations[7] = make_pair(ENE_HL, vector < glm::vec2> {	glm::vec2(2  / 16.f, 0.5f)}); //HIT LEFT
	animations[8] = make_pair(ENE_DR, vector < glm::vec2> {	glm::vec2(14 / 16.f, 0.0f), glm::vec2(15 / 16.f, 0.0f)}); //DEAD RIGHT
	animations[9] = make_pair(ENE_DL, vector < glm::vec2> {	glm::vec2(1  / 16.f, 0.5f), glm::vec2(0.0f, 0.5f)}); //DEAD LEFT

	characters[0] = new Player();
	characters[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "Resources/Sprites/Abadede/abadede_enemy_1.png", animations, glm::ivec2(P_WIDTH, P_HEIGHT), glm::vec2(1 / 16.f, 0.5f), IA_PLAYER, mainPlayer);
	characters[0]->setPosition(glm::vec2(1200, INIT_PLAYER_Y_TILES + 20));
	characters[0]->setTileMap(collision);//channge for scenario when collision load is diseabled
	characters[0]->set_X_max_min(max_x, min_x);

	//Jack
	animations.clear();
	animations.resize(10);

	animations[0] = make_pair(ENE_SR, vector < glm::vec2> {	glm::vec2(0.0f, 0.0f), glm::vec2(1 / 17.f, 0.0f), glm::vec2(2 / 17.f, 0.0f)}); //STAND RIGHT
	animations[1] = make_pair(ENE_SL, vector < glm::vec2> {	glm::vec2(16 / 17.f, 0.5f), glm::vec2(15 / 17.f, 0.5f), glm::vec2(14 / 17.f, 0.5f)}); //STAND LEFT
	animations[2] = make_pair(ENE_MR, vector < glm::vec2> {	glm::vec2(3  / 17.f, 0.0f), glm::vec2(4  / 17.f, 0.0f), glm::vec2(5  / 17.f, 0.0f), glm::vec2(6  / 17.f, 0.0f)}); //MOVE RIGHT
	animations[3] = make_pair(ENE_ML, vector < glm::vec2> {	glm::vec2(13 / 17.f, 0.5f), glm::vec2(12 / 17.f, 0.5f), glm::vec2(11 / 17.f, 0.5f), glm::vec2(10 / 17.f, 0.5f)}); //MOVE LEFT
	animations[4] = make_pair(ENE_PR, vector < glm::vec2> {	glm::vec2(7  / 17.f, 0.0f), glm::vec2(8  / 17.f, 0.0f), glm::vec2(9  / 17.f, 0.0f), glm::vec2(10 / 17.f, 0.0f), glm::vec2(11 / 17.f, 0.0f), glm::vec2(12 / 17.f, 0.0f)}); //PUNCH RIGHT
	animations[5] = make_pair(ENE_PL, vector < glm::vec2> {	glm::vec2(9  / 17.f, 0.5f), glm::vec2(8  / 17.f, 0.5f), glm::vec2(7  / 17.f, 0.5f), glm::vec2(6  / 17.f, 0.5f), glm::vec2(5  / 17.f, 0.5f), glm::vec2(4 / 17.f, 0.5f)}); //PUNCH LEFT
	animations[6] = make_pair(ENE_HR, vector < glm::vec2> {	glm::vec2(14 / 17.f, 0.0f)}); //HIT RIGHT								
	animations[7] = make_pair(ENE_HL, vector < glm::vec2> {	glm::vec2(2  / 17.f, 0.5f)}); //HIT LEFT
	animations[8] = make_pair(ENE_DR, vector < glm::vec2> {	glm::vec2(15 / 17.f, 0.5f), glm::vec2(16 / 17.f, 0.5f)}); //DEAD RIGHT
	animations[9] = make_pair(ENE_DL, vector < glm::vec2> {	glm::vec2(1  / 17.f, 0.0f), glm::vec2(0.0f, 0.0f)}); //DEAD LEFT

	characters[1] = new Player();
	characters[1]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "Resources/Sprites/Jack/jack_enemy_3.png", animations, glm::ivec2(P_WIDTH, P_HEIGHT), glm::vec2(1 / 17.f, 0.5f), IA_PLAYER, mainPlayer);
	characters[1]->setPosition(glm::vec2(1200, INIT_PLAYER_Y_TILES + 30));
	characters[1]->setTileMap(collision);//channge for scenario when collision load is diseabled
	characters[1]->set_X_max_min(max_x, min_x);

	//Zamza
	animations.clear();
	animations.resize(10);

	animations[0] = make_pair(ENE_SR, vector < glm::vec2> {	glm::vec2(0.0f, 0.0f), glm::vec2(1 / 16.f, 0.0f), glm::vec2(2 / 16.f, 0.0f)}); //STAND RIGHT
	animations[1] = make_pair(ENE_SL, vector < glm::vec2> {	glm::vec2(15 / 16.f, 0.5f), glm::vec2(14 / 16.f, 0.5f), glm::vec2(13 / 16.f, 0.5f)}); //STAND LEFT
	animations[2] = make_pair(ENE_MR, vector < glm::vec2> {	glm::vec2(3  / 16.f, 0.0f), glm::vec2(4  / 16.f, 0.0f), glm::vec2(5  / 16.f, 0.0f), glm::vec2(6  / 16.f, 0.0f), glm::vec2(7 / 16.f, 0.0f), glm::vec2(8 / 16.f, 0.0f)}); //MOVE RIGHT
	animations[3] = make_pair(ENE_ML, vector < glm::vec2> {	glm::vec2(12 / 16.f, 0.5f), glm::vec2(11 / 16.f, 0.5f), glm::vec2(10 / 16.f, 0.5f), glm::vec2(9  / 16.f, 0.5f), glm::vec2(8 / 16.f, 0.5f), glm::vec2(7 / 16.f, 0.5f)}); //MOVE LEFT
	animations[4] = make_pair(ENE_PR, vector < glm::vec2> {	glm::vec2(9  / 16.f, 0.0f), glm::vec2(10 / 16.f, 0.0f), glm::vec2(11 / 16.f, 0.0f)}); //PUNCH RIGHT
	animations[5] = make_pair(ENE_PL, vector < glm::vec2> {	glm::vec2(6  / 16.f, 0.5f), glm::vec2(5  / 16.f, 0.5f), glm::vec2(4  / 16.f, 0.5f)}); //PUNCH LEFT
	animations[6] = make_pair(ENE_HR, vector < glm::vec2> {	glm::vec2(13 / 16.f, 0.0f)}); //HIT RIGHT								
	animations[7] = make_pair(ENE_HL, vector < glm::vec2> {	glm::vec2(2  / 16.f, 0.5f)}); //HIT LEFT
	animations[8] = make_pair(ENE_DR, vector < glm::vec2> {	glm::vec2(14 / 16.f, 0.0f), glm::vec2(15 / 16.f, 0.0f)}); //DEAD RIGHT
	animations[9] = make_pair(ENE_DL, vector < glm::vec2> {	glm::vec2(1  / 16.f, 0.5f), glm::vec2(0.0f, 0.5f)}); //DEAD LEFT

	characters[2] = new Player();
	characters[2]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "Resources/Sprites/Zamza/zamza_enemy_2.png", animations, glm::ivec2(P_WIDTH, P_HEIGHT), glm::vec2(1 / 16.f, 0.5f), IA_PLAYER, mainPlayer);
	characters[2]->setPosition(glm::vec2(1200, INIT_PLAYER_Y_TILES + 80));
	characters[2]->setTileMap(collision);//channge for scenario when collision load is diseabled
	characters[2]->set_X_max_min(max_x, min_x);

}

void Level::setMainPlayer() {
	
	vector<pair<int, vector<glm::vec2>>> animations;
	
	if (mainPlayer != NULL) {
		delete mainPlayer;
	}

	mainPlayer = new Player();

	if (userPlayer == RYU) {
		//Ryu
		animations.resize(14);

		animations[0] = make_pair(MAIN_SR, vector < glm::vec2 > {	glm::vec2(8 / 27.f, 0.0f), glm::vec2(9 / 27.f, 0.0f), glm::vec2(10 / 27.f, 0.0f), glm::vec2(11 / 27.f, 0.0f)}); //STAND_RIGHT
		animations[1] = make_pair(MAIN_SL, vector < glm::vec2 > {	glm::vec2(18 / 27.f, 0.5f), glm::vec2(17 / 27.f, 0.5f), glm::vec2(16 / 27.f, 0.5f), glm::vec2(15 / 27.f, 0.5f)}); //STAND_LEFT
		animations[2] = make_pair(MAIN_MR, vector < glm::vec2 > {	glm::vec2(0.0f, 0.0f), glm::vec2(2 / 27.f, 0.0f), glm::vec2(2 / 27.f, 0.0f), glm::vec2(3 / 27.f, 0.0f), glm::vec2(4 / 27.f, 0.0f)}); //MOVE_RIGHT
		animations[3] = make_pair(MAIN_ML, vector < glm::vec2 > {	glm::vec2(26 / 27.f, 0.5f), glm::vec2(25 / 27.f, 0.5f), glm::vec2(24 / 27.f, 0.5f), glm::vec2(23 / 27.f, 0.5f), glm::vec2(22 / 27.f, 0.5f)}); //MOVE_LEFT
		animations[4] = make_pair(MAIN_KR, vector < glm::vec2 > {	glm::vec2(12 / 27.f, 0.0f), glm::vec2(13 / 27.f, 0.0f), glm::vec2(14 / 27.f, 0.0f)}); //KICK_RIGHT
		animations[5] = make_pair(MAIN_KL, vector < glm::vec2 > {	glm::vec2(14 / 27.f, 0.5f), glm::vec2(13 / 27.f, 0.5f), glm::vec2(12 / 27.f, 0.5f)}); //KICK_LEFT
		animations[6] = make_pair(MAIN_PR, vector < glm::vec2 > {	glm::vec2(5 / 27.f, 0.0f), glm::vec2(6 / 27.f, 0.0f), glm::vec2(7 / 27.f, 0.0f)}); //PUNCH_RIGHT
		animations[7] = make_pair(MAIN_PL, vector < glm::vec2 > {	glm::vec2(21 / 27.f, 0.5f), glm::vec2(20 / 27.f, 0.5f), glm::vec2(19 / 27.f, 0.5f)}); //PUNCH_LEFT
		animations[8] = make_pair(MAIN_SPR, vector < glm::vec2 > {	glm::vec2(15 / 27.f, 0.0f), glm::vec2(16 / 27.f, 0.0f), glm::vec2(17 / 27.f, 0.0f), glm::vec2(18 / 27.f, 0.0f), glm::vec2(19 / 27.f, 0.0f)}); //SPECIAL RIGHT
		animations[9] = make_pair(MAIN_SPL, vector < glm::vec2 > {	glm::vec2(11 / 27.f, 0.5f), glm::vec2(10 / 27.f, 0.5f), glm::vec2(9 / 27.f, 0.5f), glm::vec2(8 / 27.f, 0.5f), glm::vec2(7 / 27.f, 0.5f)}); //SPECIAL LEFT
		animations[10] = make_pair(MAIN_HR, vector < glm::vec2> {	glm::vec2(20 / 27.f, 0.0f), glm::vec2(21 / 27.f, 0.0f), glm::vec2(22 / 27.f, 0.0f), glm::vec2(23 / 27.f, 0.0f)}); // Hit right
		animations[11] = make_pair(MAIN_HL, vector < glm::vec2> {	glm::vec2(6 / 27.f, 0.5f), glm::vec2(5 / 27.f, 0.5f), glm::vec2(4 / 27.f, 0.5f), glm::vec2(3 / 27.f, 0.5f)}); //hit left
		animations[12] = make_pair(MAIN_HR, vector < glm::vec2> {	glm::vec2(24 / 27.f, 0.0f), glm::vec2(25 / 27.f, 0.0f), glm::vec2(26 / 27.f, 0.0f)}); // Hit right
		animations[13] = make_pair(MAIN_HL, vector < glm::vec2> {	glm::vec2(2 / 27.f, 0.5f), glm::vec2(1 / 27.f, 0.5f), glm::vec2(0.0f, 0.5f)}); //hit left

		mainPlayer->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "Resources/Sprites/Ryu/ryu.png", animations, glm::ivec2(P_WIDTH, P_HEIGHT), glm::vec2(1 / 27.f, 0.5f), USER_PLAYER, NULL);
		mainPlayer->setPosition(glm::vec2(INIT_PLAYER_X_TILES, INIT_PLAYER_Y_TILES));
		mainPlayer->setTileMap(collision); //channge for scenario when collision load is diseabled
		mainPlayer->set_X_max_min(max_x, min_x);
	}
	else if (userPlayer == BISON) {
		//Bison
		animations.clear();
		animations.resize(14);

		animations[0] = make_pair(MAIN_SR, vector < glm::vec2> { glm::vec2(0.0f, 0.0f), glm::vec2(1 / 29.f, 0.0f), glm::vec2(2 / 29.f, 0.0f)}); //STAND_RIGHT
		animations[1] = make_pair(MAIN_SL, vector < glm::vec2> { glm::vec2(28 / 29.f, 0.5f), glm::vec2(27 / 29.f, 0.5f), glm::vec2(26 / 29.f, 0.5f)}); //STAND_LEFT
		animations[2] = make_pair(MAIN_MR, vector < glm::vec2> { glm::vec2(3 / 29.f, 0.0f), glm::vec2(4 / 29.f, 0.0f), glm::vec2(5 / 29.f, 0.0f), glm::vec2(6 / 29.f, 0.0f)}); //MOVE_RIGHT
		animations[3] = make_pair(MAIN_ML, vector < glm::vec2> { glm::vec2(25 / 29.f, 0.5f), glm::vec2(24 / 29.f, 0.5f), glm::vec2(23 / 29.f, 0.5f), glm::vec2(22 / 29.f, 0.5f)}); //MOVE_LEFT
		animations[4] = make_pair(MAIN_KR, vector < glm::vec2> { glm::vec2(11 / 29.f, 0.0f), glm::vec2(12 / 29.f, 0.0f), glm::vec2(13 / 29.f, 0.0f), glm::vec2(14 / 29.f, 0.0f)}); //KICK_RIGHT
		animations[5] = make_pair(MAIN_KL, vector < glm::vec2> { glm::vec2(17 / 29.f, 0.5f), glm::vec2(16 / 29.f, 0.5f), glm::vec2(15 / 29.f, 0.5f), glm::vec2(14 / 29.f, 0.5f)}); //KICK_LEFT
		animations[6] = make_pair(MAIN_PR, vector < glm::vec2> { glm::vec2(7 / 29.f, 0.0f), glm::vec2(8 / 29.f, 0.0f), glm::vec2(9 / 29.f, 0.0f), glm::vec2(10 / 29.f, 0.0f)}); //PUNCH_RIGHT
		animations[7] = make_pair(MAIN_PL, vector < glm::vec2> { glm::vec2(21 / 29.f, 0.5f), glm::vec2(20 / 29.f, 0.5f), glm::vec2(19 / 29.f, 0.5f), glm::vec2(18 / 29.f, 0.5f)}); //PUNCH_LEFT
		animations[8] = make_pair(MAIN_SPR, vector < glm::vec2> {glm::vec2(15 / 29.f, 0.0f), glm::vec2(16 / 29.f, 0.0f), glm::vec2(17 / 29.f, 0.0f), glm::vec2(18 / 29.f, 0.0f), glm::vec2(19 / 29.f, 0.0f), glm::vec2(20 / 29.f, 0.0f), glm::vec2(21 / 29.f, 0.0f), glm::vec2(22 / 29.f, 0.0f)}); //SPECIAL_RIGHT
		animations[9] = make_pair(MAIN_SPL, vector < glm::vec2> {glm::vec2(13 / 29.f, 0.5f), glm::vec2(12 / 29.f, 0.5f), glm::vec2(11 / 29.f, 0.5f), glm::vec2(10 / 29.f, 0.5f), glm::vec2(9 / 29.f, 0.5f), glm::vec2(8 / 29.f, 0.0f), glm::vec2(7 / 29.f, 0.0f), glm::vec2(6 / 29.F, 0.0f)}); //SPECIAL_RIGHT
		animations[10] = make_pair(MAIN_HR, vector < glm::vec2> {glm::vec2(23 / 29.f, 0.0f), glm::vec2(24 / 29.f, 0.0f), glm::vec2(25 / 29.f, 0.0f)}); // Hit right
		animations[11] = make_pair(MAIN_HL, vector < glm::vec2> {glm::vec2(5 / 29.f, 0.5f), glm::vec2(4 / 29.f, 0.5f), glm::vec2(3 / 29.f, 0.5f)}); //hit left
		animations[12] = make_pair(MAIN_DR, vector < glm::vec2> {glm::vec2(26 / 29.f, 0.0f), glm::vec2(27 / 29.f, 0.0f), glm::vec2(28 / 29.f, 0.0f)}); // Hit right
		animations[13] = make_pair(MAIN_DL, vector < glm::vec2> {glm::vec2(2 / 29.f, 0.5f), glm::vec2(1 / 29.f, 0.5f), glm::vec2(0.0f, 0.5f)}); //hit left


		mainPlayer->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "Resources/Sprites/Bison/bison.png", animations, glm::ivec2(P_WIDTH, P_HEIGHT), glm::vec2(1 / 29.f, 0.5f), USER_PLAYER, NULL);
		mainPlayer->setPosition(glm::vec2(INIT_PLAYER_X_TILES, INIT_PLAYER_Y_TILES));
		mainPlayer->setTileMap(collision);//channge for scenario when collision load is diseabled
		mainPlayer->set_X_max_min(max_x, min_x);
	}
	else if (userPlayer == HONDA) {
		//Honda

		animations.clear();
		animations.resize(14);

		animations[0] = make_pair(MAIN_SR, vector < glm::vec2> { glm::vec2(0.00f, 0.0f), glm::vec2(1 / 29.f, 0.0f), glm::vec2(2  / 29.f, 0.0f), glm::vec2(3  / 29.f, 0.0f)}); //STAND_RIGHT
		animations[1] = make_pair(MAIN_SL, vector < glm::vec2> { glm::vec2(28 / 29.f, 0.5f), glm::vec2(27 / 29.f, 0.5f), glm::vec2(26 / 29.f, 0.5f), glm::vec2(25 / 29.f, 0.5f)}); //STAND_LEFT
		animations[2] = make_pair(MAIN_MR, vector < glm::vec2> { glm::vec2(4  / 29.f, 0.0f), glm::vec2(5  / 29.f, 0.0f), glm::vec2(6  / 29.f, 0.0f), glm::vec2(7 / 29.f, 0.0f)}); //MOVE_RIGHT
		animations[3] = make_pair(MAIN_ML, vector < glm::vec2> { glm::vec2(24 / 29.f, 0.5f), glm::vec2(23 / 29.f, 0.5f), glm::vec2(22 / 29.f, 0.5f), glm::vec2(21 / 29.f, 0.5f)}); //MOVE_LEFT
		animations[4] = make_pair(MAIN_KR, vector < glm::vec2> { glm::vec2(11 / 29.f, 0.0f), glm::vec2(12 / 29.f, 0.0f), glm::vec2(13 / 29.f, 0.0f)}); //KICK_RIGHT
		animations[5] = make_pair(MAIN_KL, vector < glm::vec2> { glm::vec2(17 / 29.f, 0.5f), glm::vec2(16 / 29.f, 0.5f), glm::vec2(15 / 29.f, 0.5f)}); //KICK_LEFT
		animations[6] = make_pair(MAIN_PR, vector < glm::vec2> { glm::vec2(8  / 29.f, 0.0f), glm::vec2(9  / 29.f, 0.0f), glm::vec2(10 / 29.f, 0.0f)}); //PUNCH_RIGHT
		animations[7] = make_pair(MAIN_PL, vector < glm::vec2> { glm::vec2(20 / 29.f, 0.5f), glm::vec2(19 / 29.f, 0.5f), glm::vec2(18 / 29.f, 0.5f)}); //PUNCH_LEFT
		animations[8] = make_pair(MAIN_SPR, vector < glm::vec2> {glm::vec2(14 / 29.f, 0.0f), glm::vec2(15 / 29.f, 0.0f), glm::vec2(16 / 29.f, 0.0f), glm::vec2(17 / 29.f, 0.0f), glm::vec2(18 / 29.f, 0.0f), glm::vec2(19 / 29.f, 0.0f), glm::vec2(20 / 29.f, 0.0f)}); //SPECIAL_RIGHT
		animations[9] = make_pair(MAIN_SPL, vector < glm::vec2> {glm::vec2(14 / 29.f, 0.5f), glm::vec2(13 / 29.f, 0.5f), glm::vec2(12 / 29.f, 0.5f), glm::vec2(11 / 29.f, 0.5f), glm::vec2(10 / 29.f, 0.0f), glm::vec2(9 / 29.f, 0.0f), glm::vec2(8 / 29.F, 0.0f)}); //SPECIAL_RIGHT
		animations[10] = make_pair(MAIN_HR, vector < glm::vec2> {glm::vec2(21 / 29.f, 0.0f), glm::vec2(22 / 29.f, 0.0f), glm::vec2(23 / 29.f, 0.0f)}); // Hit right
		animations[11] = make_pair(MAIN_HL, vector < glm::vec2> {glm::vec2(7  / 29.f, 0.5f), glm::vec2(6  / 29.f, 0.5f), glm::vec2(5  / 29.f, 0.5f)}); //hit left
		animations[12] = make_pair(MAIN_DR, vector < glm::vec2> {glm::vec2(24 / 29.f, 0.0f), glm::vec2(25 / 29.f, 0.0f), glm::vec2(26 / 29.f, 0.0f), glm::vec2(27 / 29.f, 0.0f), glm::vec2(28 / 29.f, 0.0f)}); // Hit right
		animations[13] = make_pair(MAIN_DL, vector < glm::vec2> {glm::vec2(4  / 29.f, 0.5f), glm::vec2(3  / 29.f, 0.5f), glm::vec2(2  / 29.F, 0.5f), glm::vec2(1 / 29.F, 0.5f), glm::vec2(0.0f, 0.5f)}); //hit left

		mainPlayer->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "Resources/Sprites/Honda/honda.png", animations, glm::ivec2(P_WIDTH, P_HEIGHT), glm::vec2(1 / 29.f, 0.5f), USER_PLAYER, NULL);
		mainPlayer->setPosition(glm::vec2(INIT_PLAYER_X_TILES, INIT_PLAYER_Y_TILES));
		mainPlayer->setTileMap(collision);//channge for scenario when collision load is diseabled
		mainPlayer->set_X_max_min(max_x, min_x);
	}
}

int Level::getAttackers() 
{
	return numAttackers;
}

void Level::setAttackers(int value)
{
	numAttackers = value;
}