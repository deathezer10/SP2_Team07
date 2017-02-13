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

#include <sstream>



SceneShop::SceneShop(Application* app) :
_app(app),
textManager(&meshList[GEO_TEXT], &m_parameters[0], &modelStack, &viewStack, &projectionStack) {
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

	static bool canChangeMenu = true;
	static bool canChangeMenu2 = true;

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
	if (_menuSelected >2)
	{
		_menuSelected = 2;
	}
	if (_menuSelected < 0)
	{
		_menuSelected = 0;
	}



	else if (Application::IsKeyPressed(VK_RETURN)) {
		switch (_menuSelected) {
		case 0:
			SceneManager::getInstance()->changeScene(new Assignment03(_app)); // Change Scene
			break;
		case 1:
			glfwSetWindowShouldClose(glfwGetCurrentContext(), true); // Toggle this to true
			break;
		}
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
	std::string title = "Title";
	std::string option1 = "Option 1";
	std::string option2 = "Option 2";


	textManager.renderTextOnScreen(UIManager::Text(title, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
	textManager.renderTextOnScreen(UIManager::Text(newLine, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
	textManager.renderTextOnScreen(UIManager::Text(newLine, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
	textManager.renderTextOnScreen(UIManager::Text(option1, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
	textManager.renderTextOnScreen(UIManager::Text(newLine, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
	textManager.renderTextOnScreen(UIManager::Text(option2, Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
	

	textManager.reset();
}

void SceneShop::RenderMesh(Mesh *mesh, bool enableLight) {
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if (enableLight) {
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else {
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if (mesh->textureID > 0) {
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else {
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}

	mesh->Render(); //this line should only be called once

	if (mesh->textureID > 0) {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

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
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
