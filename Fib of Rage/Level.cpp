#include "Level.h"
												
Level::Level(int left, int right, int bottom, int top) : Scene(left, right, bottom, top)
{
	
}

Level::Level()
{
}

Level::~Level()
{

}

void Level::init()
{
	
}

void Level::render()
{
	Scene::render();
}

void Level::update(int deltaTime) {
	Scene::update(deltaTime);
}

void Level::enableCollisionView(bool state) {
	showCollisions = state;
}

bool Level::getCollisionView() {
	return showCollisions;
}


/*
void Level::restartLevel(int player) 
{ 
	if (player != userPlayer) {
		userPlayer = player;
		setMainPlayer();
	}
	mainPlayer->setPosition(glm::vec2(INIT_PLAYER_X_TILES, INIT_PLAYER_Y_TILES)); 
	int offsetY = 10;
	int offsetX = 200;
	for each(Player *player in characters) {
		player->setPosition(glm::vec2(600 + offsetX, INIT_PLAYER_Y_TILES + offsetY));
		offsetY += 10;
		offsetX += 200;
	}
	
	setCamera(glm::ivec4(0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, 0));
	projection = glm::ortho(float(cameraLeft), float(cameraRight), float(cameraBottom), float(cameraTop));
}

void Level::restartLevel2(int player)
{

	if (player != userPlayer) {
		userPlayer = player;
		setMainPlayer();
	}
	mainPlayer->setPosition(glm::vec2(INIT_PLAYER_X_TILES, INIT_PLAYER_Y_TILES));
	int offsetY = 10;
	int offsetX = 200;
	for each(Player *player in characters) {
		player->setPosition(glm::vec2(600 + offsetX, INIT_PLAYER_Y_TILES + offsetY));
		offsetY += 10;
		offsetX += 200;
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
	characters[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "Resources/Sprites/Abadede/abadede_enemy_1.png", animations, glm::ivec2(P_WIDTH, P_HEIGHT), glm::vec2(1 / 16.f, 0.5f), IA_PLAYER, mainPlayer,A);
	characters[0]->setPosition(glm::vec2(1200, INIT_PLAYER_Y_TILES + 20));
	characters[0]->setTileMap(collisions[level]);//channge for scenario when collision load is diseabled
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
	characters[1]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "Resources/Sprites/Jack/jack_enemy_3.png", animations, glm::ivec2(P_WIDTH, P_HEIGHT), glm::vec2(1 / 17.f, 0.5f), IA_PLAYER, mainPlayer,J);
	characters[1]->setPosition(glm::vec2(1200, INIT_PLAYER_Y_TILES + 30));
	characters[1]->setTileMap(collisions[level]);//channge for scenario when collision load is diseabled
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
	characters[2]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "Resources/Sprites/Zamza/zamza_enemy_2.png", animations, glm::ivec2(P_WIDTH, P_HEIGHT), glm::vec2(1 / 16.f, 0.5f), IA_PLAYER, mainPlayer,Z);
	characters[2]->setPosition(glm::vec2(1200, INIT_PLAYER_Y_TILES + 80));
	characters[2]->setTileMap(collisions[level]);//channge for scenario when collision load is diseabled
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

		mainPlayer->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "Resources/Sprites/Ryu/ryu.png", animations, glm::ivec2(P_WIDTH, P_HEIGHT), glm::vec2(1 / 27.f, 0.5f), USER_PLAYER, NULL,RYUT);
		mainPlayer->setPosition(glm::vec2(INIT_PLAYER_X_TILES, INIT_PLAYER_Y_TILES));
		mainPlayer->setTileMap(collisions[level]); //channge for scenario when collision load is diseabled
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


		mainPlayer->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "Resources/Sprites/Bison/bison.png", animations, glm::ivec2(P_WIDTH, P_HEIGHT), glm::vec2(1 / 29.f, 0.5f), USER_PLAYER, NULL,BISONT);
		mainPlayer->setPosition(glm::vec2(INIT_PLAYER_X_TILES, INIT_PLAYER_Y_TILES));
		mainPlayer->setTileMap(collisions[level]);//channge for scenario when collision load is diseabled
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

		mainPlayer->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "Resources/Sprites/Honda/honda.png", animations, glm::ivec2(P_WIDTH, P_HEIGHT), glm::vec2(1 / 29.f, 0.5f), USER_PLAYER, NULL,HONDAT);
		mainPlayer->setPosition(glm::vec2(INIT_PLAYER_X_TILES, INIT_PLAYER_Y_TILES));
		mainPlayer->setTileMap(collisions[level]);//channge for scenario when collision load is diseabled
		mainPlayer->set_X_max_min(max_x, min_x);
	}
}
*/
