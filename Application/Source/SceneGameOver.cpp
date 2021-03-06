#include "Application.h"

#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "Mtx44.h"
#include "shader.hpp"

#include "SceneManager.h"
#include "SceneGameover.h"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "SceneMainMenu.h"
#include "PlayerDataManager.h"
#include "SceneCargoShip.h"
#include "SceneDogfight.h"
#include "SceneBoss.h"
#include <sstream>


SceneGameover::SceneGameover(string title, TYPE_MENU type, TYPE_SCENE previousScene, int currency) : Scene(SCENE_GAMEOVER_VICTORY) {
	_previousScene = previousScene;
	_menuSelected = 0;
	_menuType = type;
	_title = title;
	_currencyEarned = currency;
}

SceneGameover::~SceneGameover() {


}

void SceneGameover::Init() {

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

	Mtx44 projection;
	projection.SetToPerspective(45.f, 5.12f / 3.6f, 0.1f, 2000.f); // far clipping
	projectionStack.LoadMatrix(projection);

	//remove all glGenBuffers, glBindBuffer, glBufferData code
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

	meshList[GEO_UI_DEFEAT] = MeshBuilder::GenerateQuad("UI", Color(1, 1, 1), 1.f);
	meshList[GEO_UI_DEFEAT]->textureID = LoadTGA("Image/ui_defeat.tga");

	//	meshList[GEO_UI] = MeshBuilder::GenerateQuad("UI", Color(1, 1, 1), 1.f);
	//	meshList[GEO_UI]->textureID = LoadTGA("Image/shop/ui.tga");

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
	camera.allowMovement(false);

	PlayerDataManager::getInstance()->getPlayerData()->currency += _currencyEarned;
	PlayerDataManager::getInstance()->SaveData();
}

void SceneGameover::Update(double dt) {

	camera.Update(dt);

	GLFWwindow* window = glfwGetCurrentContext();

	if (!glfwGetKey(window, GLFW_KEY_UP) && !glfwGetKey(window, GLFW_KEY_DOWN)) {
		canPressUpDown = true;
	}

	///////////////////////////////////////////////vvvvvvvvvvvvvvvvvvARROW CONTROL FOR VICTORY MENUvvvvvvvvvvvvvvvvvvvvvvv/////////////////////////////////////////
	if (_menuType == MENU_VICTORY&&_previousScene != SCENE_BOSS) {
		if (glfwGetKey(window, GLFW_KEY_UP) && canPressUpDown) {
			if (_menuSelected >= 1) {
				_menuSelected -= 1;
				canPressUpDown = false;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) && canPressUpDown) {
			if (_menuSelected <= 1) {
				_menuSelected += 1;
				canPressUpDown = false;
			}
		}
	}

	if (_menuType == MENU_VICTORY&&_previousScene == SCENE_BOSS) {
		if (glfwGetKey(window, GLFW_KEY_UP) && canPressUpDown) {
			if (_menuSelected >= 1) {
				_menuSelected -= 1;
				canPressUpDown = false;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) && canPressUpDown) {
			if (_menuSelected <= 1) {
				_menuSelected = 1;
				canPressUpDown = false;
			}
		}
	}
	///////////////////////////////////////////^^^^^^^^^^^^^^^^^^^^^ARROW CONTROL FOR VICTORY MENU^^^^^^^^^^^^^^^^^^^/////////////////////////////////////////////

	////////////////////////////////////////////////////////vvvvvvvvvvvvARROWS CONTROL FOR GAME OVER MENUvvvvvvvvvvvvvvvvvvvvvv//////////////////////////////
	if (_menuType == MENU_GAMEOVER) {
		if (glfwGetKey(window, GLFW_KEY_UP) && canPressUpDown) {
			if (_menuSelected >= 1) {
				_menuSelected -= 1;
				canPressUpDown = false;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) && canPressUpDown) {
			if (_menuSelected <= 1) {
				_menuSelected += 1;
				canPressUpDown = false;
			}
		}
	}
	/////////////////////////////////////////////////////////^^^^^^^^^^^^^^^^ARROWS CONTROL FOR GAME OVER MENU^^^^^^^^^^^^^^//////////////////////////////


	if (!glfwGetKey(window, GLFW_KEY_ENTER)) {
		canPressEnter = true;
	}

	//////////////////////////////////////////////////////////vvvvvvvvvvvvvCHANGE GAME STATEvvvvvvvvvvvvvvvvvv//////////////////////////////////////////////
	if (glfwGetKey(window, GLFW_KEY_ENTER) && canPressEnter && _menuType == MENU_GAMEOVER) {
		switch (_menuSelected) {
		case 0:
			SceneManager::getInstance()->changeScene(createScene(_previousScene)); // Change to previous Scene
			break;
		case 1:
			SceneManager::getInstance()->changeScene(new SceneMainMenu());
			break;
		case 2:
			glfwSetWindowShouldClose(glfwGetCurrentContext(), true); // Flag application to quit
			break;
		}

		canPressEnter = false;

	}
	if (glfwGetKey(window, GLFW_KEY_ENTER) && canPressEnter && _menuType == MENU_VICTORY&&_previousScene == Scene::SCENE_BOSS) {
		switch (_menuSelected) {
		case 0:
			SceneManager::getInstance()->changeScene(new SceneMainMenu());
			break;
		case 1:
			glfwSetWindowShouldClose(glfwGetCurrentContext(), true); // Flag application to quit
			break;
		}
		canPressEnter = false;
	}

	if (glfwGetKey(window, GLFW_KEY_ENTER) && canPressEnter && _menuType == MENU_VICTORY) {
		switch (_menuSelected) {
		case 0:
			switch (_previousScene) {
			case Scene::SCENE_TUTORIAL:
				SceneManager::getInstance()->changeScene(new SceneDogfight()); // Change to previous Scene
				break;
			case Scene::SCENE_DOGFIGHT:
				SceneManager::getInstance()->changeScene(new SceneCargoShip()); // Change to previous Scene
				break;
			case Scene::SCENE_CARGOSHIP:
				SceneManager::getInstance()->changeScene(new SceneBoss()); // Change to previous Scene
				break;
			case Scene::SCENE_BOSS:
				break;
			default:
				break;
			}
			break;
		case 1:
			SceneManager::getInstance()->changeScene(new SceneMainMenu());
			break;
		case 2:
			glfwSetWindowShouldClose(glfwGetCurrentContext(), true); // Flag application to quit
			break;
		}
		canPressEnter = false;

	}
	/////////////////////////////////////////////////////////////////^^^^^^^^^^^^^^CHANGE GAME STATE^^^^^^^^^^^^^^^^^^//////////////////////////////////////////////


}

void SceneGameover::Render() {
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


	float winWidth = (float)Application::windowWidth() / 10;
	float winHeight = (float)Application::windowHeight() / 10;
	Vector3 tileScale(25, 8, 23);/*
	glDisable(GL_DEPTH_TEST);
	textManager.RenderMeshOnScreen(meshList[GEO_UI], winWidth * 0.50f, winHeight * 0.28f, Vector3(90, 0, 0), tileScale);
	glEnable(GL_DEPTH_TEST);
	*/

	std::string newLine = "";
	std::string title = _title;

	std::string option1 = "Option 1";
	std::string option2 = "Option 2";

	switch (_menuType) {

	case TYPE_MENU::MENU_GAMEOVER:
		glDisable(GL_DEPTH_TEST);

		textManager.RenderMeshOnScreen(meshList[GEO_UI_DEFEAT], winWidth * 0.505f, winHeight * 0.40f, Vector3(90, 0, 0), tileScale);
		option1 = (_menuSelected == 0) ? ">Restart<" : "Restart";
		option2 = (_menuSelected == 2) ? ">Quit<" : "Quit";
		break;
	case TYPE_MENU::MENU_VICTORY:
		switch (_previousScene) {
		case Scene::SCENE_BOSS:
			option1 = (_menuSelected == 0) ? ">Main Menu<" : "Main Menu";
			option2 = (_menuSelected == 1) ? ">Quit<" : "Quit";
			break;
		default:
			option1 = (_menuSelected == 0) ? ">Next level<" : "Next level";
			option2 = (_menuSelected == 2) ? ">Quit<" : "Quit";
			break;
		}
	}


	
	


	textManager.renderTextOnScreen(UIManager::Text(title, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
	textManager.renderTextOnScreen(UIManager::Text(newLine, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
	textManager.renderTextOnScreen(UIManager::Text(newLine, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
	textManager.renderTextOnScreen(UIManager::Text(option1, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
	textManager.renderTextOnScreen(UIManager::Text(newLine, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));


	if (_menuType == MENU_VICTORY&&_previousScene == SCENE_BOSS) {
		textManager.renderTextOnScreen(UIManager::Text(newLine, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
		textManager.renderTextOnScreen(UIManager::Text(option2, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
	}

	if (_previousScene == Scene::SCENE_BOSS&&_menuType == MENU_GAMEOVER) {
		std::string option3 = "Option 3";
		option3 = (_menuSelected == 1) ? ">Main Menu<" : "Main Menu";
		textManager.renderTextOnScreen(UIManager::Text(option3, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
		textManager.renderTextOnScreen(UIManager::Text(newLine, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
		textManager.renderTextOnScreen(UIManager::Text(option2, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
		textManager.renderTextOnScreen(UIManager::Text(newLine, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
	}
	if (_previousScene != Scene::SCENE_BOSS&&_menuType == MENU_VICTORY) {
		std::string option3 = "Option 3";
		option3 = (_menuSelected == 1) ? ">Main Menu<" : "Main Menu";
		textManager.renderTextOnScreen(UIManager::Text(option3, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
		textManager.renderTextOnScreen(UIManager::Text(newLine, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
		textManager.renderTextOnScreen(UIManager::Text(option2, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
	}

	if (_menuType == MENU_GAMEOVER&&_previousScene != SCENE_BOSS) {
		std::string option3 = "Option 3";
		option3 = (_menuSelected == 1) ? ">Main Menu<" : "Main Menu";
		textManager.renderTextOnScreen(UIManager::Text(option3, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
		textManager.renderTextOnScreen(UIManager::Text(newLine, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
		textManager.renderTextOnScreen(UIManager::Text(option2, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
	}

	std::string option4 = "Currency earned: ";
	option4.append(std::to_string(_currencyEarned));
	textManager.renderTextOnScreen(UIManager::Text(option4, Color(1, 1, 1), UIManager::ANCHOR_TOP_CENTER));

	textManager.reset();
}

void SceneGameover::RenderSkybox() {

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

void SceneGameover::Exit() {
	for (int i = 0; i < NUM_GEOMETRY; ++i) {
		if (meshList[i] != nullptr) {
			delete meshList[i];
		}
	}

	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
