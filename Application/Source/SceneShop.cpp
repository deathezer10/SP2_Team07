#include "Application.h"

#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "Mtx44.h"
#include "shader.hpp"

#include "SceneManager.h"
#include "SceneMainMenu.h"
#include "SceneShop.h"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "Assignment03.h"
#include "UIManager.h"
#include <sstream>


std::string ShipType[4] = { "FG-6", "SF-1", "DF-6", "Trident-A10" };

SceneShop::SceneShop()
{
	_menuSelected = 0;
}

SceneShop::~SceneShop() {


}

void SceneShop::Init() {

	//Load vertex and fragment shadersm
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
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
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	camera.disableMouse();

	Mtx44 projection;
	projection.SetToPerspective(45.f, 5.12f / 3.6f, 0.1f, 2000.f); // far clipping
	projectionStack.LoadMatrix(projection);

	//remove all glGenBuffers, glBindBuffer, glBufferData code
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);

	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball1", Color(1, 1, 0), 16, 16, 0.25f);

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//skybox//front.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//skybox//back.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//skybox//top.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//skybox//bottom.tga");

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image///skybox/left.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image///skybox/right.tga");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//arial.tga");
	textManager.LoadFontWidth("Image//arial.csv");

	meshList[GEO_CRATE] = MeshBuilder::GenerateOBJ("crate", "OBJ/crate.obj");
	meshList[GEO_CRATE]->textureID = LoadTGA("Image/crate.tga");

	//OBJ SPACESHIP
	meshList[GEO_FG6] = MeshBuilder::GenerateOBJ("crate", "OBJ/fG6.obj");
	meshList[GEO_FG6]->textureID = LoadTGA("Image/fG6.tga");

	meshList[GEO_SF1] = MeshBuilder::GenerateOBJ("crate", "OBJ/sf1.obj");
	meshList[GEO_SF1]->textureID = LoadTGA("Image/sf1.tga");

	meshList[GEO_DF6] = MeshBuilder::GenerateOBJ("crate", "OBJ/df6.obj");
	meshList[GEO_DF6]->textureID = LoadTGA("Image/df6.tga");

	meshList[GEO_A10] = MeshBuilder::GenerateOBJ("crate", "OBJ/a10.obj");
	meshList[GEO_A10]->textureID = LoadTGA("Image/a10.tga");

	// Lighting 1
	light[0].position.Set(4, 4, 0);
	light[0].type = Light::LIGHT_SPOT;
	light[0].color.Set(1, 1, 1);
	light[0].power = 1;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(70));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 1.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

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

	glUniform1i(m_parameters[U_NUMLIGHTS], 1); // Make sure you pass uniform parameters after glUseProgram()


	pData = PlayerDataManager::getInstance()->getPlayerData();
}

void SceneShop::Update(double dt) {

	_dt = dt;

	if (Application::IsKeyPressed('1')) {
		glEnable(GL_CULL_FACE);
	}
	if (Application::IsKeyPressed('2')) {
		glDisable(GL_CULL_FACE);
	}
	if (Application::IsKeyPressed('3')) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	}
	if (Application::IsKeyPressed('4')) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode
	}

	camera.Update(dt);

	if (!Application::IsKeyPressed(VK_UP))
	{
		canChangeMenu = true;
	}
	if (Application::IsKeyPressed(VK_UP) && canChangeMenu == true)
	{
		_menuSelected -= 1;
		canChangeMenu = false;
	}
	if (!Application::IsKeyPressed(VK_DOWN))
	{
		canChangeMenu2 = true;
	}
	if (Application::IsKeyPressed(VK_DOWN) && canChangeMenu2 == true)
	{
		_menuSelected += 1;
		canChangeMenu2 = false;
	}
	if (_menuSelected > 6)
	{
		_menuSelected = 6;
	}
	if (_menuSelected < 0)
	{
		_menuSelected = 0;
	}

	//Changing ships-----------------------------------------------------
	//const std::string ShipType[4] = { "FG6", "SF1", "DF6", "A10" };






	if (!Application::IsKeyPressed(VK_RIGHT) && !Application::IsKeyPressed(VK_LEFT))
	{
		ChangeShip = true;
	}

	if (Application::IsKeyPressed(VK_RIGHT) && ChangeShip == true && _menuSelected == 0)
	{
		column += 1;
		ChangeShip = false;
	}
	else if (Application::IsKeyPressed(VK_LEFT) && ChangeShip == true && _menuSelected == 0)
	{
		column -= 1;
		ChangeShip = false;
	}

	if (column < 0)
	{
		column = 0;
	}
	if (column > 3)
	{
		column = 3;
	}

	if (!Application::IsKeyPressed(VK_RETURN))
	{
		CanPress = true;
	}
	if (Application::IsKeyPressed(VK_RETURN) && CanPress) {

		switch (_menuSelected) {
		case 0:
			break;
		case 1:
			if (column == 0 && pData->currency > 5)
			{
				pData->FG6_Bullet_damage++;
				pData->currency -= 5;
				PlayerDataManager::getInstance()->SaveData(column);
			}
			if (column == 1)
			{
				pData->SF1_Bullet_damage++;
				PlayerDataManager::getInstance()->SaveData(column);
			}
			if (column == 2)
			{
				pData->DF6_Bullet_damage++;
				PlayerDataManager::getInstance()->SaveData(column);
			}
			if (column == 3)
			{
				pData->A10_Bullet_damage++;
				PlayerDataManager::getInstance()->SaveData(column);
			}
			break;
		case 2:
			if (column == 0)
			{
				pData->FG6_Bullet_speed++;
				PlayerDataManager::getInstance()->SaveData(column);
			}
			if (column == 1)
			{
				pData->SF1_Bullet_speed++;
				PlayerDataManager::getInstance()->SaveData(column);
			}
			if (column == 2)
			{
				pData->DF6_Bullet_speed++;
				PlayerDataManager::getInstance()->SaveData(column);
			}
			if (column == 3)
			{
				pData->A10_Bullet_speed++;
				PlayerDataManager::getInstance()->SaveData(column);
			}
			break;
		case 3:
			if (column == 0)
			{
				pData->FG6_Bullet_ROF++;
				PlayerDataManager::getInstance()->SaveData(column);
			}
			if (column == 1)
			{
				pData->SF1_Bullet_ROF++;
				PlayerDataManager::getInstance()->SaveData(column);
			}
			if (column == 2)
			{
				pData->DF6_Bullet_ROF++;
				PlayerDataManager::getInstance()->SaveData(column);
			}
			if (column == 3)
			{
				pData->A10_Bullet_ROF++;
				PlayerDataManager::getInstance()->SaveData(column);
			}
			break;
		case 4:
			if (column == 0)
			{
				pData->FG6_Shield_hp++;
				PlayerDataManager::getInstance()->SaveData(column);
			}
			if (column == 1)
			{
				pData->SF1_Shield_hp++;
				PlayerDataManager::getInstance()->SaveData(column);
			}
			if (column == 2)
			{
				pData->DF6_Shield_hp++;
				PlayerDataManager::getInstance()->SaveData(column);
			}
			if (column == 3)
			{
				pData->A10_Shield_hp++;
				PlayerDataManager::getInstance()->SaveData(column);
			}
			break;
		case 5:
			if (column == 0)
			{
				pData->FG6_shield_recharge_rate++;
				PlayerDataManager::getInstance()->SaveData(column);
			}
			if (column == 1)
			{
				pData->SF1_shield_recharge_rate++;
				PlayerDataManager::getInstance()->SaveData(column);
			}
			if (column == 2)
			{
				pData->DF6_shield_recharge_rate++;
				PlayerDataManager::getInstance()->SaveData(column);
			}
			if (column == 3)
			{
				pData->A10_shield_recharge_rate++;
				PlayerDataManager::getInstance()->SaveData(column);
			}
			break;
		case 6:

			SceneManager::getInstance()->changeScene(new SceneMainMenu()); // Change Scene
			break;

		}
		CanPress = false;
	}

}

void SceneShop::Render() {
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Light 1
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


	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y,
		camera.position.z, camera.target.x, camera.target.y,
		camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();


	RenderSkybox();


	std::string newLine = "";
	std::string title = "Welcome to the item shop";
	//name
	std::string option1 = "NAME";
	//Upgrades
	std::string option2 = "Option 1";
	std::string option3 = "Option 2";
	std::string option4 = "Option 3";
	std::string option5 = "Option 4";
	std::string option6 = "Option 5";
	//Back
	std::string option7 = "Option 6";

	//Currency
	std::string option8 = "Money: $";
	option8 += std::to_string(pData->currency);

	option1 = (_menuSelected == 0) ? ">Selected Ship: " : "Selected Ship: ";
	option1 += ShipType[column];
	option2 = (_menuSelected == 1) ? ">Bullet Damage: " : "Bullet Damage: ";
	if (column == 0)
	{
		option2 += std::to_string(pData->FG6_Bullet_damage);
	}
	if (column == 1)
	{
		option2 += std::to_string(pData->SF1_Bullet_damage);
	}
	if (column == 2)
	{
		option2 += std::to_string(pData->DF6_Bullet_damage);
	}
	if (column == 3)
	{
		option2 += std::to_string(pData->A10_Bullet_damage);
	}

	option3 = (_menuSelected == 2) ? ">Bullet Speed: " : "Bullet Speed: ";
	if (column == 0)
	{
		option3 += std::to_string(pData->FG6_Bullet_speed);
	}
	if (column == 1)
	{
		option3 += std::to_string(pData->SF1_Bullet_speed);
	}
	if (column == 2)
	{
		option3 += std::to_string(pData->DF6_Bullet_speed);
	}
	if (column == 3)
	{
		option3 += std::to_string(pData->A10_Bullet_speed);
	}
	option4 = (_menuSelected == 3) ? ">Bullet Rate of Fire:" : "Bullet Rate of Fire: ";
	if (column == 0)
	{
		option4 += std::to_string(pData->FG6_Bullet_ROF);
	}
	if (column == 1)
	{
		option4 += std::to_string(pData->SF1_Bullet_ROF);
	}
	if (column == 2)
	{
		option4 += std::to_string(pData->DF6_Bullet_ROF);
	}
	if (column == 3)
	{
		option4 += std::to_string(pData->A10_Bullet_ROF);
	}
	option5 = (_menuSelected == 4) ? ">Shield Hp: " : "Shield Hp: ";
	if (column == 0)
	{
		option5 += std::to_string(pData->FG6_Shield_hp);
	}
	if (column == 1)
	{
		option5 += std::to_string(pData->SF1_Shield_hp);
	}
	if (column == 2)
	{
		option5 += std::to_string(pData->DF6_Shield_hp);
	}
	if (column == 3)
	{
		option5 += std::to_string(pData->A10_Shield_hp);
	}
	option6 = (_menuSelected == 5) ? ">Shield Recharge Rate:" : "Shield Recharge Rate: ";
	if (column == 0)
	{
		option6 += std::to_string(pData->FG6_shield_recharge_rate);
	}
	if (column == 1)
	{
		option6 += std::to_string(pData->SF1_shield_recharge_rate);
	}
	if (column == 2)
	{
		option6 += std::to_string(pData->DF6_shield_recharge_rate);
	}
	if (column == 3)
	{
		option6 += std::to_string(pData->A10_shield_recharge_rate);
	}
	option7 = (_menuSelected == 6) ? ">Back" : "Back";


	textManager.renderTextOnScreen(UIManager::Text(title, Color(1, 1, 1), UIManager::ANCHOR_TOP_CENTER));
	textManager.renderTextOnScreen(UIManager::Text(newLine, Color(1, 1, 1), UIManager::ANCHOR_TOP_CENTER));
	textManager.renderTextOnScreen(UIManager::Text(option8, Color(1, 1, 1), UIManager::ANCHOR_TOP_CENTER));
	rotShip.y += 45 * _dt;
	int spaceShipX = Application::_windowWidth / 20;
	int spaceShipY = Application::_windowHeight / 15;

	Vector3 scaleShip(35, 35, 35);


	modelStack.PushMatrix();
	switch (column)
	{
	case 0:
		textManager.RenderMeshOnScreen(meshList[GEO_FG6], spaceShipX, spaceShipY, rotShip, scaleShip);
		break;
	case 1:
		textManager.RenderMeshOnScreen(meshList[GEO_SF1], spaceShipX, spaceShipY, rotShip, scaleShip);
		break;
	case 2:
		textManager.RenderMeshOnScreen(meshList[GEO_DF6], spaceShipX, spaceShipY, rotShip, scaleShip);
		break;
	case 3:
		textManager.RenderMeshOnScreen(meshList[GEO_A10], spaceShipX, spaceShipY, rotShip, scaleShip);
		break;
	}
	
	modelStack.PopMatrix();

	textManager.renderTextOnScreen(UIManager::Text(newLine, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
	textManager.renderTextOnScreen(UIManager::Text(newLine, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
	textManager.renderTextOnScreen(UIManager::Text(option1, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
	textManager.renderTextOnScreen(UIManager::Text(newLine, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
	textManager.renderTextOnScreen(UIManager::Text(option2, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
	textManager.renderTextOnScreen(UIManager::Text(newLine, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
	textManager.renderTextOnScreen(UIManager::Text(option3, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
	textManager.renderTextOnScreen(UIManager::Text(newLine, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
	textManager.renderTextOnScreen(UIManager::Text(option4, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
	textManager.renderTextOnScreen(UIManager::Text(newLine, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
	textManager.renderTextOnScreen(UIManager::Text(option5, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
	textManager.renderTextOnScreen(UIManager::Text(newLine, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
	textManager.renderTextOnScreen(UIManager::Text(option6, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
	textManager.renderTextOnScreen(UIManager::Text(newLine, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
	textManager.renderTextOnScreen(UIManager::Text(option7, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));



	textManager.reset();
}

void SceneShop::RenderSkybox() {

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

void SceneShop::Exit() {
	for (int i = 0; i < NUM_GEOMETRY; ++i) {
		if (meshList[i] != nullptr) {
			delete meshList[i];
		}
	}

	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
