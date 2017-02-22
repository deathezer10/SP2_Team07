#include "Application.h"

#include "GL\glew.h"
#include "GLFW\glfw3.h"

#include "shader.hpp"

#include "PlayerDataManager.h"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "Tdummy.h"
#include "XF02.h"

#include "SceneDogfight.h"
#include "SceneManager.h"
#include "SceneMainMenu.h"
#include "SceneGameOver.h"

#include <sstream>


SceneDogfight::SceneDogfight() : Scene(SCENE_DOGFIGHT) {
}

SceneDogfight::~SceneDogfight() {}

void SceneDogfight::Init() {

	pData = PlayerDataManager::getInstance()->getPlayerData();

	PlayerStat* gg = PlayerDataManager::getInstance()->getPlayerStats();

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

	meshList[GEO_BULLET02] = MeshBuilder::GenerateOBJ("bullet", "OBJ/bullet.obj");
	meshList[GEO_BULLET02]->textureID = LoadTGA("Image/enemybullet.tga");

	meshList[GEO_RING] = MeshBuilder::GenerateOBJ("ring", "OBJ/ring.obj");
	meshList[GEO_RING]->textureID = LoadTGA("Image/ring.tga");

	meshList[GEO_RADAR_BACKGROUND] = MeshBuilder::GenerateQuad("radar bg", Color(0, 1, 0));
	meshList[GEO_RADAR_ENEMY] = MeshBuilder::GenerateQuad("radar enemy icon", Color(1, 0, 0));
	meshList[GEO_RADAR_PLAYER] = MeshBuilder::GenerateQuad("radar player icon", Color(0, 0, 1));

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

	meshList[GEO_TDUMMY] = MeshBuilder::GenerateOBJ("enemy", "OBJ/drone.obj");
	meshList[GEO_TDUMMY]->textureID = LoadTGA("Image/drone.tga");

	meshList[GEO_XF2] = MeshBuilder::GenerateOBJ("enemy", "OBJ/xf02.obj");
	meshList[GEO_XF2]->textureID = LoadTGA("Image/xf02.tga");


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

	meshList[GEO_MENU_BACKGROUND] = MeshBuilder::GenerateQuad("UI Background", Color(1, 1, 1), 10, 12);

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image/arial.tga");
	textManager.LoadFontWidth("Image/arial.csv");


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

	const size_t rockAmount = 200;
	const float randRange = 500;

	// Create interactable rocks
	for (size_t i = 0; i < rockAmount; i++) {
		Rock* gg = new Rock(this, Vector3(Math::RandFloatMinMax(-randRange, randRange), Math::RandFloatMinMax(-randRange, randRange), Math::RandFloatMinMax(-randRange, randRange)));
		gg->setCollision(true);
		objBuilder.createObject(gg);
	}


	// Create NPCs
	objBuilder.createObject(new XF02(this, Vector3(-25, 0, 100)), td_OBJ_TYPE::TYPE_ENEMY);
	objBuilder.createObject(new XF02(this, Vector3(25, -20, 100)), td_OBJ_TYPE::TYPE_ENEMY);
	objBuilder.createObject(new XF02(this, Vector3(0, -25, 100)), td_OBJ_TYPE::TYPE_ENEMY);
	objBuilder.createObject(new XF02(this, Vector3(0, 20, 100)), td_OBJ_TYPE::TYPE_ENEMY);

	// Disable controls at start of tutorial
	//camera.allowYaw(false);
	//camera.allowPitch(false);
	//skillManager.disableSkills();


	const int powerCount = 50;

	for (size_t i = 0; i < rockAmount; i++) {
		PowerUp* gg = new PowerUp(this, Vector3(Math::RandFloatMinMax(-randRange, randRange), Math::RandFloatMinMax(-randRange, randRange), Math::RandFloatMinMax(-randRange, randRange)), static_cast<PowerUp::PowerType>(Math::RandIntMinMax(0, 3)));
		objBuilder.createObject(gg);
	}

}

void SceneDogfight::Update(double dt) {

	_dt = (float)dt;
	_elapsedTime += _dt;



	pauseManager.UpdatePauseMenu((float)dt);

	if (pauseManager.isPaused()){
		return;
	}

	if (Application::IsKeyPressed(VK_F1)) {
		glEnable(GL_CULL_FACE);
	}
	if (Application::IsKeyPressed(VK_F2)) {
		glDisable(GL_CULL_FACE);
	}
	if (Application::IsKeyPressed(VK_F3)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	}
	if (Application::IsKeyPressed(VK_F4)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode
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

		waypoint.RotateTowards(*XF02::NearestXF02Pos);

		textManager.queueRenderText(UIManager::Text("Eliminate all enemies!", Color(1, 0, 1), UIManager::ANCHOR_TOP_CENTER));

		objCount << "Enemies Left: " << XF02::XF02Count;
		textManager.queueRenderText(UIManager::Text(objCount.str(), Color(1, 1, 1), UIManager::ANCHOR_TOP_RIGHT));

		objDist << "Distance: " << (int)((*XF02::NearestXF02Pos) - camera.position).Length() << "m";
		textManager.queueRenderText(UIManager::Text(objDist.str(), Color(1, 0, 1), UIManager::ANCHOR_TOP_CENTER));

		if (XF02::XF02Count == 0) {
			++currentObjective;
		}

		break;

	case 1:

		objCount << "(Still WIP)";
		textManager.queueRenderText(UIManager::Text(objCount.str(), Color(1, 0, 1), UIManager::ANCHOR_TOP_CENTER));
		break;

	}


}

void SceneDogfight::Render() {
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();

	if (Application::IsKeyPressed(MK_RBUTTON)) {
		viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z,
			-camera.playerView.x, camera.playerView.y, -camera.playerView.z,
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
	modelStack.Rotate(-camera.getYaw(), 0, 1, 0);
	modelStack.Rotate(-camera.getPitch(), 0, 0, 1);
	modelStack.Rotate(-camera.getRoll(), 1, 0, 0);
	RenderMesh(meshList[GEO_SPACESHIP], true);
	modelStack.PopMatrix();

	// Render all interactable objects
	objBuilder.renderObjects();


	// Anything after this is not rendered
	if (pauseManager.isPaused()){
		pauseManager.RenderPauseMenu();
		return;
	}



	textManager.renderPlayerHUD();

	textManager.renderTextOnScreen(UIManager::Text("<Objective>", Color(1, 1, 1), UIManager::ANCHOR_TOP_CENTER));

	// Render all pending text onto screen
	textManager.dequeueText();
	textManager.reset();
}

void SceneDogfight::RenderSkybox() {

	modelStack.PushMatrix();

	modelStack.Translate(camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -1000);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(camera.skyboxSize, 1, camera.skyboxSize);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 1000);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(camera.skyboxSize, 1, camera.skyboxSize);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-1000, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(camera.skyboxSize, 1, camera.skyboxSize);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1000, 0, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(camera.skyboxSize, 1, camera.skyboxSize);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 1000, 0);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(camera.skyboxSize, 1, camera.skyboxSize);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -1000, 0);
	modelStack.Scale(camera.skyboxSize, 1, camera.skyboxSize);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();

	modelStack.PopMatrix();


}

void SceneDogfight::Exit() {

	for (int i = 0; i < NUM_GEOMETRY; ++i) {
		if (meshList[i] != nullptr) {
			delete meshList[i];
		}
	}

	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
