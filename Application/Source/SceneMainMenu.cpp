#include "Application.h"

#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "Mtx44.h"
#include "shader.hpp"

#include "SceneManager.h"
#include "SceneMainMenu.h"
#include "SceneShop.h"
#include "SceneTutorial.h"
#include "SceneDogfight.h"
#include "SceneCargoShip.h"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "CargoShip.h"

#include <sstream>



SceneMainMenu::SceneMainMenu() : Scene(SCENE_MAINMENU) {
	_outsideSelected = 0;
	_insideSelected = 0;
}

SceneMainMenu::~SceneMainMenu() {
}

void SceneMainMenu::Init() {

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
	camera.allowMovement(false);
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

	meshList[GEO_MAIN_MENU] = MeshBuilder::GenerateQuad("main_menu", Color(1, 1, 1), 1.f);
	meshList[GEO_MAIN_MENU]->textureID = LoadTGA("Image/mainmenu/main_menu.tga");

	meshList[GEO_SINGLE_PLAYER] = MeshBuilder::GenerateQuad("single player", Color(1, 1, 1), 1.f);
	meshList[GEO_SINGLE_PLAYER]->textureID = LoadTGA("Image/mainmenu/play2.tga");

	meshList[GEO_SINGLE_PLAYER_SELECTED] = MeshBuilder::GenerateQuad("single player", Color(1, 1, 1), 1.f);
	meshList[GEO_SINGLE_PLAYER_SELECTED]->textureID = LoadTGA("Image/mainmenu/play_selected.tga");

	meshList[GEO_SHOP] = MeshBuilder::GenerateQuad("SHOP", Color(1, 1, 1), 1.f);
	meshList[GEO_SHOP]->textureID = LoadTGA("Image/mainmenu/shop.tga");

	meshList[GEO_SHOP_SELECTED] = MeshBuilder::GenerateQuad("SHOP", Color(1, 1, 1), 1.f);
	meshList[GEO_SHOP_SELECTED]->textureID = LoadTGA("Image/mainmenu/shop_selected.tga");

	meshList[GEO_EXIT] = MeshBuilder::GenerateQuad("exit", Color(1, 1, 1), 1.f);
	meshList[GEO_EXIT]->textureID = LoadTGA("Image/mainmenu/exit.tga");

	meshList[GEO_EXIT_SELECTED] = MeshBuilder::GenerateQuad("exit", Color(1, 1, 1), 1.f);
	meshList[GEO_EXIT_SELECTED]->textureID = LoadTGA("Image/mainmenu/exit_selected.tga");

	meshList[GEO_TITLE] = MeshBuilder::GenerateQuad("TITLE", Color(1, 1, 1), 1.f);
	meshList[GEO_TITLE]->textureID = LoadTGA("Image/mainmenu/SPACE_FIGHTER2.tga");

	meshList[GEO_HANGER] = MeshBuilder::GenerateQuad("HANGER", Color(1, 1, 1), 1.f);
	meshList[GEO_HANGER]->textureID = LoadTGA("Image/shop/hanger.tga");

	meshList[GEO_MONEY] = MeshBuilder::GenerateOBJ("MONEY", "OBJ/money.obj");
	meshList[GEO_MONEY]->textureID = LoadTGA("Image/mainmenu/money.tga");

	meshList[GEO_PLANET] = MeshBuilder::GenerateOBJ("PLANET", "OBJ/planet.obj");
	meshList[GEO_PLANET]->textureID = LoadTGA("Image/mainmenu/planet.tga");

	meshList[GEO_GALAXY] = MeshBuilder::GenerateOBJ("galaxy", "OBJ/galaxy.obj");
	meshList[GEO_GALAXY]->textureID = LoadTGA("Image/mainmenu/galaxy.tga");



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

	////create cargoship
	//_CargoShip = new CargoShip(this, Vector3(0, 0, 20));
	//_CargoShip->setCollision(true);
	//objBuilder.createObject(_CargoShip, td_OBJ_TYPE::TYPE_OBJECTIVE);
}

void SceneMainMenu::Update(double dt) {

	_dt = (float)dt;
	objBuilder.objInteractor.updateInteraction();

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


	if (!Application::IsKeyPressed(VK_UP)&&!Application::IsKeyPressed(VK_LEFT)) {
		canPressUp = true;
	}
	if (Application::IsKeyPressed(VK_UP) && canPressUp == true || Application::IsKeyPressed(VK_LEFT) && canPressUp == true) {
		if (isMenuOutside)
			_outsideSelected--;
		else
			_insideSelected--;

		canPressUp = false;
	}
	if (!Application::IsKeyPressed(VK_DOWN)&&!Application::IsKeyPressed(VK_RIGHT)) {
		canPressDown = true;
	}
	if ((Application::IsKeyPressed(VK_DOWN) && canPressDown == true)||(Application::IsKeyPressed(VK_RIGHT) && canPressDown == true)) {
		if (isMenuOutside)
			_outsideSelected++;
		else
			_insideSelected++;

		canPressDown = false;
	}

	_insideSelected = Math::Clamp<int>(_insideSelected, 0, 4);
	_outsideSelected = Math::Clamp<int>(_outsideSelected, 0, 2);


	if (!Application::IsKeyPressed(VK_RETURN)) {
		canPressEnter = true;
	}

	if (Application::IsKeyPressed(VK_RETURN) && canPressEnter == true) {

		if (isMenuOutside) {

			switch (_outsideSelected) {
			case 0:
				isMenuOutside = false;
				//SceneManager::getInstance()->changeScene(new Assignment03()); // Change Scene
				break;
			case 1:
				//insert scene transition to SceneMainMenu
				SceneManager::getInstance()->changeScene(new SceneShop()); // Change Scene
				break;
			case 2:
				glfwSetWindowShouldClose(glfwGetCurrentContext(), true); // Flag game to exit
				break;
			}
		}
		else {
			switch (_insideSelected) {
			case 0: // Tutorial
				SceneManager::getInstance()->changeScene(new SceneTutorial()); // Change Scene
				break;
			case 1: // Dog fight
				SceneManager::getInstance()->changeScene(new SceneDogfight()); // Change Scene
				break;
			case 2: // Payload
				SceneManager::getInstance()->changeScene(new SceneCargoShip()); // Change Scene
				break;
			case 3: // Boss
				SceneManager::getInstance()->changeScene(new SceneCargoShip()); // Change Scene
				break;
			case 4: // Back button
				_insideSelected = 0;
				isMenuOutside = true;
				break;
			}
		}

		canPressEnter = false;
	}

}

void SceneMainMenu::Render() {
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




	std::string newLine = "";
	std::string title = "Title";
	std::string option1 = "Option 1";
	std::string option2 = "Option 2";
	std::string option3 = "Option 3";

	float winWidth = (float)Application::windowWidth() / 10;
	float winHeight = (float)Application::windowHeight() / 10;

	textManager.RenderMeshOnScreen(meshList[GEO_MAIN_MENU], winWidth / 2, winHeight / 2, Vector3(90, 0, 0), Vector3(52, 1, 52));

	if (isMenuOutside) {

		title = "SPACE FIGHTER 27";
		option1 = (_outsideSelected == 0) ? ">Play<" : "Play";
		option2 = (_outsideSelected == 1) ? ">Shop<" : "Shop";
		option3 = (_outsideSelected == 2) ? ">Quit<" : "Quit";

		glDisable(GL_DEPTH_TEST);

		Vector3 selectionScale(5, 20, 2);
		Vector3 tileScale(20, 80, 8);

		textManager.RenderMeshOnScreen(meshList[GEO_SINGLE_PLAYER], winWidth * 0.25f, winHeight * 0.65f, Vector3(90, 0, 0), selectionScale);
		textManager.RenderMeshOnScreen(meshList[GEO_SHOP], winWidth * 0.75f, winHeight * 0.65f, Vector3(90, 0, 0), selectionScale);
		textManager.RenderMeshOnScreen(meshList[GEO_EXIT], winWidth * 0.75f, winHeight * 0.30f, Vector3(90, 0, 0), selectionScale);
		textManager.RenderMeshOnScreen(meshList[GEO_TITLE], winWidth * 0.50f, winHeight * 0.90f, Vector3(90, 0, 0), tileScale);

		if (_outsideSelected == 0) {
			rotate_planet += 20 * _dt;
			textManager.RenderMeshOnScreen(meshList[GEO_SINGLE_PLAYER_SELECTED], winWidth * 0.25f, winHeight * 0.65f, Vector3(90, 0, 0), selectionScale);
		}

		if (_outsideSelected == 1) {
			rotate_money += 90 * _dt;
			textManager.RenderMeshOnScreen(meshList[GEO_SHOP_SELECTED], winWidth * 0.75f, winHeight * 0.65f, Vector3(90, 0, 0), selectionScale);
		}

		if (_outsideSelected == 2) {
			rotate += 90 * _dt;
			textManager.RenderMeshOnScreen(meshList[GEO_EXIT_SELECTED], winWidth * 0.75f, winHeight * 0.30f, Vector3(90, 0, 0), selectionScale);
		}

		Vector3 galaxyScale(5, 5, 5);
		rotate += 10 * _dt;
		textManager.RenderMeshOnScreen(meshList[GEO_GALAXY], winWidth * 0.75f, winHeight * 0.20f, Vector3(0, 0, rotate), galaxyScale);

		rotate_money += 40 * _dt;
		textManager.RenderMeshOnScreen(meshList[GEO_MONEY], winWidth * 0.75f, winHeight * 0.55f, Vector3(0, 90 + rotate_money, 0), galaxyScale);
		glEnable(GL_DEPTH_TEST);

		rotate_planet += 40 * _dt;
		textManager.RenderMeshOnScreen(meshList[GEO_PLANET], winWidth * 0.25f, winHeight * 0.50f, Vector3(0, rotate_planet, -10), galaxyScale);
		glEnable(GL_DEPTH_TEST);

		

	}
	else {

		glDisable(GL_DEPTH_TEST);
		std::string option1 = "Tutorial";
		std::string option2 = "level 1";
		std::string option3 = "level 2";
		std::string option4 = "level 3";
		std::string option5 = "back";

		title = "Level Select";
		option1 = (_insideSelected == 0) ? ">0) Tutorial<" : "0) Tutorial";
		option2 = (_insideSelected == 1) ? ">1) Dog Fight<" : "1) Dog Fight";
		option3 = (_insideSelected == 2) ? ">2) Escort Cargo Ship<" : "2) Escort Cargo Ship";
		option4 = (_insideSelected == 3) ? ">3) Attack on Destroyer-01<" : "3) Attack on Destroyer-01";
		option5 = (_insideSelected == 4) ? ">back<" : "back";

		textManager.renderTextOnScreen(UIManager::Text(title, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
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
	}

	objBuilder.renderObjects();
	textManager.reset();
}

void SceneMainMenu::Exit() {
	for (int i = 0; i < NUM_GEOMETRY; ++i) {
		if (meshList[i] != nullptr) {
			delete meshList[i];
		}
	}

	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
