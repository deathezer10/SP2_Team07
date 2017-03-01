#include "Application.h"

#include "GL\glew.h"
#include "GLFW\glfw3.h"

#include "shader.hpp"

#include "PlayerDataManager.h"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "Tdummy.h"

#include "SceneTutorial.h"
#include "SceneManager.h"
#include "SceneMainMenu.h"
#include "SceneGameOver.h"

#include <sstream>


SceneTutorial::SceneTutorial() : Scene(SCENE_TUTORIAL) {
}

SceneTutorial::~SceneTutorial() {}

void SceneTutorial::Init() {

	pData = PlayerDataManager::getInstance()->getPlayerData();

	//Load vertex and fragment shaders
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");

	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");

	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	glUseProgram(m_programID);


	// Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND); // Enable blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	for (int i = 0; i < NUM_GEOMETRY; ++i) {
		meshList[i] = nullptr;
	}

	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");

	// Init Camera
	camera.Init(Vector3(0, 0, 0), Vector3(0, 0, 10), Vector3(0, 1, 0));

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 2000.f); // far clipping
	projectionStack.LoadMatrix(projection);

	//remove all glGenBuffers, glBindBuffer, glBufferData code
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image/skybox/front.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image/skybox/back.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image/skybox/top.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image/skybox/bottom.tga");

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image/skybox/left.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image/skybox/right.tga");

	meshList[GEO_BULLET] = MeshBuilder::GenerateOBJ("bullet", "OBJ/bullet.obj");
	meshList[GEO_BULLET]->textureID = LoadTGA("Image/playerbullet.tga");

	meshList[GEO_RING] = MeshBuilder::GenerateOBJ("ring", "OBJ/ring.obj");
	meshList[GEO_RING]->textureID = LoadTGA("Image/ring.tga");

	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("powerup", Color(.12f, .18f, .32f));
	meshList[GEO_CUBE]->material.kAmbient.Set(1.0f, 1.0f, 0.0f);
	meshList[GEO_CUBE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_CUBE]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_CUBE]->material.kShininess = 1.0f;

	switch (pData->currentFighter) {

	case 0:
		meshList[GEO_SPACESHIP] = MeshBuilder::GenerateOBJ("spaceship", "OBJ/fG6.obj");
		meshList[GEO_SPACESHIP]->textureID = LoadTGA("Image/fG6.tga");
		break;

	case 1:
		meshList[GEO_SPACESHIP] = MeshBuilder::GenerateOBJ("spaceship", "OBJ/sf1.obj");
		meshList[GEO_SPACESHIP]->textureID = LoadTGA("Image/sf1.tga");
		break;

	case 2:
		meshList[GEO_SPACESHIP] = MeshBuilder::GenerateOBJ("spaceship", "OBJ/df6.obj");
		meshList[GEO_SPACESHIP]->textureID = LoadTGA("Image/df6.tga");
		break;

	case 3:
		meshList[GEO_SPACESHIP] = MeshBuilder::GenerateOBJ("spaceship", "OBJ/a10.obj");
		meshList[GEO_SPACESHIP]->textureID = LoadTGA("Image/a10.tga");
		break;

	}

	meshList[GEO_MISSILE] = MeshBuilder::GenerateOBJ("missile", "OBJ/missile.obj");
	meshList[GEO_MISSILE]->textureID = LoadTGA("Image/missile.tga");

	meshList[GEO_TDUMMY] = MeshBuilder::GenerateOBJ("enemy", "OBJ/drone.obj");
	meshList[GEO_TDUMMY]->textureID = LoadTGA("Image/drone.tga");

	meshList[GEO_REGEN] = MeshBuilder::GenerateOBJ("regen", "OBJ/regen.obj");
	meshList[GEO_REGEN]->textureID = LoadTGA("Image/regen.tga");

	meshList[GEO_SPEED] = MeshBuilder::GenerateOBJ("speed", "OBJ/speed.obj");
	meshList[GEO_SPEED]->textureID = LoadTGA("Image/speed.tga");

	meshList[GEO_BARRAGE] = MeshBuilder::GenerateOBJ("barrage", "OBJ/barrage.obj");
	meshList[GEO_BARRAGE]->textureID = LoadTGA("Image/barrage.tga");

	meshList[GEO_ROCK1] = MeshBuilder::GenerateOBJ("rock1", "OBJ/rock1.obj");
	meshList[GEO_ROCK1]->textureID = LoadTGA("Image/rock1.tga");

	meshList[GEO_ROCK2] = MeshBuilder::GenerateOBJ("rock2", "OBJ/Rock2.obj");
	meshList[GEO_ROCK2]->textureID = LoadTGA("Image/Rock2.tga");

	meshList[GEO_ROCK3] = MeshBuilder::GenerateOBJ("rock3", "OBJ/Rock3.obj");
	meshList[GEO_ROCK3]->textureID = LoadTGA("Image/Rock3.tga");

	meshList[GEO_ROCK4] = MeshBuilder::GenerateOBJ("rock4", "OBJ/Rock4.obj");
	meshList[GEO_ROCK4]->textureID = LoadTGA("Image/Rock4.tga");

	meshList[GEO_WAYPOINT] = MeshBuilder::GenerateOBJ("rock4", "OBJ/waypoint.obj");
	meshList[GEO_WAYPOINT]->textureID = LoadTGA("Image/waypoint.tga");

	meshList[GEO_HP_FOREGROUND] = MeshBuilder::GenerateUIQuad("Cargo HP", Color(0.0f, 0.6f, 0.0f));
	meshList[GEO_HP_FOREGROUND]->textureID = LoadTGA("Image/health_fg.tga");//texture for health

	meshList[GEO_HP_BACKGROUND] = MeshBuilder::GenerateUIQuad("Cargo HP", Color(0.8f, 0.0f, 0.0f));
	meshList[GEO_HP_BACKGROUND]->textureID = LoadTGA("Image/white_bg.tga");//transparency bg

	meshList[GEO_SHIELD_FOREGROUND] = MeshBuilder::GenerateUIQuad("Cargo HP", Color(0.0f, 0.6f, 1.0f));
	meshList[GEO_SHIELD_FOREGROUND]->textureID = LoadTGA("Image/shield_fg.tga");//texture for shield

	meshList[GEO_RADAR_BACKGROUND] = MeshBuilder::GenerateQuad("radar bg", Color(0, 0.5f, 0));
	meshList[GEO_RADAR_BACKGROUND]->textureID = LoadTGA("Image/radar.tga", true);

	meshList[GEO_RADAR_ENEMY] = MeshBuilder::GenerateQuad("radar enemy icon", Color(1, 0, 0));
	meshList[GEO_RADAR_PLAYER] = MeshBuilder::GenerateQuad("radar player icon", Color(0, 0, 1));

	meshList[GEO_DMGINDICATOR_BLUE] = MeshBuilder::GenerateQuad("dmg indicator blue", Color(1, 0, 0));
	meshList[GEO_DMGINDICATOR_BLUE]->textureID = LoadTGA("Image/dmgindicator-blue.tga");

	meshList[GEO_DMGINDICATOR_RED] = MeshBuilder::GenerateQuad("dmg indicator red", Color(1, 0, 0));
	meshList[GEO_DMGINDICATOR_RED]->textureID = LoadTGA("Image/dmgindicator-red.tga");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image/arial.tga");
	textManager.LoadFontWidth("Image/arial.csv");

	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("Collider", Color(0, 1, 0), 0.5f, 0.5f, 0.5f);
	

	// Lighting 1
	light[0].type = Light::LIGHT_SPOT;
	light[0].position.Set(0, 10, 5);
	light[0].color.Set(1, 1, 1);
	light[0].power = 2;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(25));
	light[0].cosInner = cos(Math::DegreeToRadian(15));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);


	// Lighting 2
	light[1].type = Light::LIGHT_DIRECTIONAL;
	light[1].position.Set(0, 10, -5);
	light[1].color.Set(1.0f, 1.0f, 1.0f);
	light[1].power = 1.0f;


	// Light 1
	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

	// Light 2
	glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);

	glUniform1i(m_parameters[U_NUMLIGHTS], 2); // Make sure to pass uniform parameters after glUseProgram()

	const size_t rockAmount = 250;
	const float randRange = 250;

	//// Create interactable rocks
	//for (size_t i = 0; i < rockAmount; i++) {
	//	Rock* gg = new Rock(this, Vector3(Math::RandFloatMinMax(-randRange, randRange), Math::RandFloatMinMax(-randRange, randRange), Math::RandFloatMinMax(-randRange, randRange)));
	//	gg->setCollision(true);
	//	objBuilder.createObject(gg, td_OBJ_TYPE::TYPE_OBJECTIVE);
	//}


	// Create interactable Rings
	objBuilder.createObject(new Ring(this, Vector3(0, 5, 100)), td_OBJ_TYPE::TYPE_OBJECTIVE);

	// Disable controls at start of tutorial
	camera.allowYaw(false);
	camera.allowPitch(false);
	skillManager.disableSkills();

}

void SceneTutorial::Update(double dt) {

	_dt = (float)dt;
	_elapsedTime += _dt;

	pauseManager.UpdatePauseMenu((float)dt);

	if (pauseManager.isPaused()) {
		return;
	}



	camera.Update(dt);
	objBuilder.objInteractor.updateInteraction();
	skillManager.processSkills(dt);

	// Flashlight position and direction
	light[0].position.Set(camera.position.x, camera.position.y, camera.position.z);
	light[0].spotDirection = camera.position - camera.target;

	std::ostringstream objDist;
	std::ostringstream objCount;


	// Objective Logic
	switch (currentObjective) {

	case 0: // Accelerate to collect ring
		waypoint.RotateTowards(*Ring::NearestRingPos);

		textManager.queueRenderText(UIManager::Text("Collect the Ring", Color(1, 0, 1), UIManager::ANCHOR_TOP_CENTER));
		textManager.queueRenderText(UIManager::Text("[Tip] Press [W/S] to Accelerate/Decelerate", Color(1, 1, 1), UIManager::ANCHOR_TOP_CENTER));

		objCount << "Ring(s) left: " << Ring::RingCount;
		textManager.queueRenderText(UIManager::Text(objCount.str(), Color(1, 0, 1), UIManager::ANCHOR_TOP_RIGHT));

		objDist << "Distance: " << (int)((*Ring::NearestRingPos) - camera.position).Length() << "m";
		textManager.queueRenderText(UIManager::Text(objDist.str(), Color(1, 1, 1), UIManager::ANCHOR_TOP_CENTER));

		// Transition to next objective
		if (Ring::RingCount == 0) {
			camera.setVelocity(1);
			camera.allowYaw(true);

			objBuilder.createObject(new Ring(this, Vector3(25, 5, 200)), td_OBJ_TYPE::TYPE_OBJECTIVE);
			objBuilder.createObject(new Ring(this, Vector3(-25, 5, 300)), td_OBJ_TYPE::TYPE_OBJECTIVE);

			++currentObjective;
		}
		break;

	case 1: // Yaw left/right to rotate fighter and strafe
		waypoint.RotateTowards(*Ring::NearestRingPos);

		textManager.queueRenderText(UIManager::Text("Collect the Ring", Color(1, 0, 1), UIManager::ANCHOR_TOP_CENTER));
		textManager.queueRenderText(UIManager::Text("[Tip] Press [A/D] to Strafe Left/Right", Color(1, 1, 1), UIManager::ANCHOR_TOP_CENTER));
		textManager.queueRenderText(UIManager::Text("[Tip] Mouse [Left/Right] to Turn Left/Right", Color(1, 1, 1), UIManager::ANCHOR_TOP_CENTER));
		objCount << "Ring(s) left: " << Ring::RingCount;
		textManager.queueRenderText(UIManager::Text(objCount.str(), Color(1, 0, 1), UIManager::ANCHOR_TOP_RIGHT));

		objDist << "Distance: " << (int)((*Ring::NearestRingPos) - camera.position).Length() << "m";
		textManager.queueRenderText(UIManager::Text(objDist.str(), Color(1, 1, 1), UIManager::ANCHOR_TOP_CENTER));

		// Transition to next objective
		if (Ring::RingCount == 0) {
			camera.setVelocity(1);
			camera.allowPitch(true);

			objBuilder.createObject(new Ring(this, Vector3(-25, 20, 500)), td_OBJ_TYPE::TYPE_OBJECTIVE);
			objBuilder.createObject(new Ring(this, Vector3(-25, -20, 400)), td_OBJ_TYPE::TYPE_OBJECTIVE);

			++currentObjective;
		}
		break;

	case 2: // mouse up/down to pitch and thrust up/dowm
		waypoint.RotateTowards(*Ring::NearestRingPos);

		textManager.queueRenderText(UIManager::Text("Collect the Ring", Color(1, 0, 1), UIManager::ANCHOR_TOP_CENTER));
		textManager.queueRenderText(UIManager::Text("[Tip] Mouse [Up/Down] to Look Up/Down", Color(1, 1, 1), UIManager::ANCHOR_TOP_CENTER));
		textManager.queueRenderText(UIManager::Text("[Tip] Press [Q/E] to Thrust Up/Down", Color(1, 1, 1), UIManager::ANCHOR_TOP_CENTER));
		objCount << "Ring(s) left: " << Ring::RingCount;
		textManager.queueRenderText(UIManager::Text(objCount.str(), Color(1, 0, 1), UIManager::ANCHOR_TOP_RIGHT));

		objDist << "Distance: " << (int)((*Ring::NearestRingPos) - camera.position).Length() << "m";
		textManager.queueRenderText(UIManager::Text(objDist.str(), Color(1, 1, 1), UIManager::ANCHOR_TOP_CENTER));

		// Transition to next objective
		if (Ring::RingCount == 0) {
			camera.setVelocity(1);
			skillManager.enableSkills();

			objBuilder.createObject(new Tdummy(this, Vector3(-25, 0, 600)), td_OBJ_TYPE::TYPE_ENEMY);
			objBuilder.createObject(new Tdummy(this, Vector3(25, -20, 650)), td_OBJ_TYPE::TYPE_ENEMY);
			objBuilder.createObject(new Tdummy(this, Vector3(0, -25, 600)), td_OBJ_TYPE::TYPE_ENEMY);
			objBuilder.createObject(new Tdummy(this, Vector3(0, 20, 650)), td_OBJ_TYPE::TYPE_ENEMY);
			++currentObjective;
		}
		break;

	case 3: // Eliminate all training dummies
		waypoint.RotateTowards(*Tdummy::NearestTdummyPos);

		textManager.queueRenderText(UIManager::Text("Eliminate all Training Dummies!", Color(1, 0, 1), UIManager::ANCHOR_TOP_CENTER));
		textManager.queueRenderText(UIManager::Text("[Tip] [Left-Click] to Shoot", Color(1, 1, 1), UIManager::ANCHOR_TOP_CENTER));
		textManager.queueRenderText(UIManager::Text("[Tip] Hold [SpaceBar] to fire Missile", Color(1, 1, 1), UIManager::ANCHOR_TOP_CENTER));

		objCount << "Training Dummy(s) left: " << Tdummy::TdummyCount;
		textManager.queueRenderText(UIManager::Text(objCount.str(), Color(1, 0, 1), UIManager::ANCHOR_TOP_RIGHT));

		objDist << "Distance: " << (int)((*Tdummy::NearestTdummyPos) - camera.position).Length() << "m";
		textManager.queueRenderText(UIManager::Text(objDist.str(), Color(1, 1, 1), UIManager::ANCHOR_TOP_CENTER));

		// Transition to next objective
		if (Tdummy::TdummyCount == 0) {
			camera.setVelocity(1);
			camera.Reset();
			skillManager.disableSkills();

			Rock* rock = new Rock(this, Vector3(0, 0, 100));
			rock->setCollision(true);
			objBuilder.createObject(rock, ObjectInteractor::TYPE_OBJECTIVE);
			++currentObjective;
		}

		break;

	case 4: // Crash into the asteroid
		waypoint.RotateTowards(Vector3(0, 0, 100));

		textManager.queueRenderText(UIManager::Text("Asteroids can kill you! Try crashing into it!", Color(1, 0, 1), UIManager::ANCHOR_TOP_CENTER));

		objDist << "Distance: " << (int)(Vector3(0, 0, 100) - camera.position).Length() << "m";
		textManager.queueRenderText(UIManager::Text(objDist.str(), Color(1, 1, 1), UIManager::ANCHOR_TOP_CENTER));

		// Transition to next objective
		if (Rock::RockCount == 0) {
			camera.setVelocity(1);
			skillManager.enableSkills();

			objBuilder.createObject(new PowerUp(this, Vector3(0, 0, 150), PowerUp::POWER_REGEN));

			PlayerDataManager::getInstance()->getPlayerStats()->current_shield = 0;
			PlayerDataManager::getInstance()->getPlayerStats()->current_health = 10;

			++currentObjective;
		}

		break;

	case 5: // Recover your HP
		waypoint.RotateTowards(Vector3(0, 0, 150));

		textManager.queueRenderText(UIManager::Text("Ouch! That hurts.. Pick up the [Repair Kit] to recover Full HP!", Color(1, 0, 1), UIManager::ANCHOR_TOP_CENTER));

		objDist << "Distance: " << (int)(Vector3(0, 0, 150) - camera.position).Length() << "m";
		textManager.queueRenderText(UIManager::Text(objDist.str(), Color(1, 1, 1), UIManager::ANCHOR_TOP_CENTER));

		// Player recovered, change objective!
		if (PlayerDataManager::getInstance()->getPlayerStats()->current_health >= 100) {
			camera.setVelocity(1);

			objBuilder.createObject(new PowerUp(this, Vector3(0, 0, 175), PowerUp::POWER_BARRAGE));

			for (int i = -5; i <= 5; ++i) {
				objBuilder.createObject(new Tdummy(this, Vector3(i * (float)10, 0, 300)), td_OBJ_TYPE::TYPE_ENEMY);
			}

			++currentObjective;
		}
		break;

	case 6: // Barrage all dummies
		waypoint.RotateTowards(*Tdummy::NearestTdummyPos);

		textManager.queueRenderText(UIManager::Text("Pick up the [Barrage] Power-Up and eliminate all Training Dummies!", Color(1, 0, 1), UIManager::ANCHOR_TOP_CENTER));

		objDist << "Distance: " << (int)((*Tdummy::NearestTdummyPos) - camera.position).Length() << "m";
		textManager.queueRenderText(UIManager::Text(objDist.str(), Color(1, 1, 1), UIManager::ANCHOR_TOP_CENTER));

		// Dummies eliminated
		if (Tdummy::TdummyCount == 0) {

			objBuilder.createObject(new PowerUp(this, Vector3(0, 0, 275), PowerUp::POWER_SPEEDBOOST));
			objBuilder.createObject(new Ring(this, Vector3(0, 0, 1000)), td_OBJ_TYPE::TYPE_OBJECTIVE);

			++currentObjective;
		}
		break;

	case 7: // Spped Boost to the End

		if (camera.getCurrentVelocity() != camera.getMaxVelocity()) {
			waypoint.RotateTowards(Vector3(0, 0, 275));
		}
		else {
			waypoint.RotateTowards(*Ring::NearestRingPos);
		}

		textManager.queueRenderText(UIManager::Text("Final Mission: Pick Up [Speed Boost] and reach the Checkpoint!", Color(1, 0, 1), UIManager::ANCHOR_TOP_CENTER));

		objDist << "Distance: " << (int)((*Ring::NearestRingPos) - camera.position).Length() << "m";
		textManager.queueRenderText(UIManager::Text(objDist.str(), Color(1, 1, 1), UIManager::ANCHOR_TOP_CENTER));

		// Objective Complete!
		if (Ring::RingCount == 0) {

			// Unlock level 1
			if (PlayerDataManager::getInstance()->getPlayerData()->level01_unlocked == false) {
				PlayerDataManager::getInstance()->getPlayerData()->level01_unlocked = true;
				PlayerDataManager::getInstance()->SaveData();
			}

			SceneManager::getInstance()->changeScene(new SceneGameover("Great Job! You've completed the Tutorial!", SceneGameover::TYPE_MENU::MENU_VICTORY, TYPE_SCENE::SCENE_TUTORIAL));
			return;
		}
		break;

	}


}

void SceneTutorial::Render() {
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();

	if (glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_2)) {

		Vector3 playerViewOffset = camera.position + (camera.getView().Normalized() * 3);

		viewStack.LookAt(playerViewOffset.x, playerViewOffset.y, playerViewOffset.z,
						 camera.target.x, camera.target.y, camera.target.z,
						 camera.up.x, camera.up.y, camera.up.z);
	}
	else {
		viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z,
						 camera.target.x, camera.target.y, camera.target.z,
						 camera.up.x, camera.up.y, camera.up.z);
	}

	modelStack.LoadIdentity();

	if (light[0].type == Light::LIGHT_DIRECTIONAL) {
		Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (light[0].type == Light::LIGHT_SPOT) {
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else {
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}

	if (light[1].type == Light::LIGHT_DIRECTIONAL) {
		Vector3 lightDir(light[1].position.x, light[1].position.y, light[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (light[1].type == Light::LIGHT_SPOT) {
		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else {
		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}

	RenderSkybox();

	waypoint.RenderArrow();

	// Character Transform
	modelStack.PushMatrix();
	modelStack.Translate(camera.playerView.x, camera.playerView.y - 0.5f, camera.playerView.z);
	modelStack.Rotate(-camera.getYaw() + camera.FakeYaw, 0, 1, 0);
	modelStack.Rotate(-camera.getPitch() + camera.FakePitch, 0, 0, 1);
	modelStack.Rotate(-camera.getRoll() + camera.FakeRow, 1, 0, 0);
	RenderMesh(meshList[GEO_SPACESHIP], true);
	modelStack.PopMatrix();

	// Render all interactable objects
	objBuilder.renderObjects();

	// Anything after this is not rendered
	if (pauseManager.isPaused()) {
		pauseManager.RenderPauseMenu();
		return;
	}


	textManager.renderTextOnScreen(UIManager::Text("<Objective>", Color(1, 1, 1), UIManager::ANCHOR_TOP_CENTER));

	// Render all pending text onto screen
	textManager.dequeueMesh();
	textManager.dequeueText();
	textManager.renderPlayerHUD();
	textManager.reset();
}

void SceneTutorial::RenderSkybox() {

	modelStack.PushMatrix();

	modelStack.Translate(camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);


	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -990);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(camera.skyboxSize, 1, camera.skyboxSize);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 990);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(camera.skyboxSize, 1, camera.skyboxSize);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-990, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(camera.skyboxSize, 1, camera.skyboxSize);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(990, 0, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(camera.skyboxSize, 1, camera.skyboxSize);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 990, 0);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(camera.skyboxSize, 1, camera.skyboxSize);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -990, 0);
	modelStack.Scale(camera.skyboxSize, 1, camera.skyboxSize);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();;

	modelStack.PopMatrix();

}

void SceneTutorial::Exit() {

	for (int i = 0; i < NUM_GEOMETRY; ++i) {
		if (meshList[i] != nullptr) {
			delete meshList[i];
		}
	}

	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
