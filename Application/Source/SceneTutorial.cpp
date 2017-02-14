#include "Application.h"

#include "GL\glew.h"
#include "GLFW\glfw3.h"

#include "Mtx44.h"
#include "shader.hpp"

#include "SceneTutorial.h"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

int count = 0;
bool ring = true;
SceneTutorial::SceneTutorial() :
charManager(this),
textManager(this) {
}

SceneTutorial::~SceneTutorial() {}

void SceneTutorial::Init() {

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
	camera.Init(Vector3(0, 2, 0), Vector3(0, 2, 10), Vector3(0, 1, 0));

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 2000.f); // far clipping
	projectionStack.LoadMatrix(projection);

	//remove all glGenBuffers, glBindBuffer, glBufferData code
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);

	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball1", Color(1, 1, 1), 16, 16, 0.25f);
	meshList[GEO_LIGHTBALL2] = MeshBuilder::GenerateSphere("lightball2", Color(0, 0, 1), 16, 16, 0.25f);

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

	meshList[GEO_FLOOR] = MeshBuilder::GenerateQuad("floor", Color(0, 0, 0), 250.f, 250.f, 50); // Original: 250, 250, 50
	meshList[GEO_FLOOR]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_FLOOR]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_FLOOR]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_FLOOR]->material.kShininess = 0.5f;
	meshList[GEO_FLOOR]->textureID = LoadTGA("Image/moonfloor.tga", true, true);

	meshList[GEO_BULLET] = MeshBuilder::GenerateSphere("GEO_BULLET", Color(.12f, .18f, .32f), 16, 16, 0.15f);
	meshList[GEO_BULLET]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_BULLET]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_BULLET]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_BULLET]->material.kShininess = 1.0f;

	meshList[GEO_RING] = MeshBuilder::GenerateTorus("RING", Color(.12f, .18f, .32f),18,18,2,0.5);
	meshList[GEO_RING]->material.kAmbient.Set(1.0f, 1.0f, 0.0f);
	meshList[GEO_RING]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_RING]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_RING]->material.kShininess = 1.0f;


	meshList[GEO_SLIME] = MeshBuilder::GenerateOBJ("slime", "OBJ/slime.obj");
	meshList[GEO_SLIME]->textureID = LoadTGA("Image/slime.tga");

	meshList[GEO_SPACESHIP] = MeshBuilder::GenerateOBJ("spaceship", "OBJ/spaceship.obj");
	meshList[GEO_SPACESHIP]->textureID = LoadTGA("Image/spaceship.tga");

	meshList[GEO_BUILDING] = MeshBuilder::GenerateOBJ("building", "OBJ/building.obj");
	meshList[GEO_BUILDING]->textureID = LoadTGA("Image/building.tga");

	meshList[GEO_TABLE] = MeshBuilder::GenerateOBJ("table", "OBJ/table.obj");
	meshList[GEO_TABLE]->textureID = LoadTGA("Image/table.tga");

	meshList[GEO_CHAIR] = MeshBuilder::GenerateOBJ("chair", "OBJ/chair.obj");
	meshList[GEO_CHAIR]->textureID = LoadTGA("Image/chair.tga");

	meshList[GEO_DOOR] = MeshBuilder::GenerateOBJ("door", "OBJ/door.obj");
	meshList[GEO_DOOR]->textureID = LoadTGA("Image/door.tga");

	meshList[GEO_CRATE] = MeshBuilder::GenerateOBJ("crate", "OBJ/crate.obj");
	meshList[GEO_CRATE]->textureID = LoadTGA("Image/crate.tga");

	meshList[GEO_LAMP] = MeshBuilder::GenerateOBJ("lamp", "OBJ/lamp.obj");
	meshList[GEO_LAMP]->textureID = LoadTGA("Image/lamp.tga");

	meshList[GEO_ROCK1] = MeshBuilder::GenerateOBJ("rock1", "OBJ/rock1.obj");
	meshList[GEO_ROCK1]->textureID = LoadTGA("Image/rock1.tga");



	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image/arial.tga");
	textManager.LoadFontWidth("Image/arial.csv");


	// Lighting 1
	light[0].type = Light::LIGHT_SPOT;
	light[0].position.Set(0, 10, 5);
	light[0].color.Set(1, 1, 1);
	light[0].power = 0;
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
	light[1].color.Set(0, 0.0f, 1.0f);
	light[1].power = 0.5f;


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

	//const size_t rockAmount = 50;

	//// Create interactable rocks
	//for (size_t i = 0; i < rockAmount; i++) {
	//	objBuilder.createObject(new Rock(this, Vector3(Math::RandFloatMinMax(-20, 20), 0, Math::RandFloatMinMax(-20, 20))));
	//}

	//const size_t slimeAmount = 10;

	//// Create interactable rocks
		objBuilder.createObject(new Ring(this, Vector3(Math::RandFloatMinMax(-50, 50), 0, Math::RandFloatMinMax(-50, 50))));
		objBuilder.createObject(new Ring(this, Vector3(Math::RandFloatMinMax(-50, 50), 0, Math::RandFloatMinMax(-50, 50))));
		objBuilder.createObject(new Ring(this, Vector3(Math::RandFloatMinMax(-50, 50), 0, Math::RandFloatMinMax(-50, 50))));
		objBuilder.createObject(new Ring(this, Vector3(Math::RandFloatMinMax(-50, 50), 0, Math::RandFloatMinMax(-50, 50))));
		objBuilder.createObject(new Ring(this, Vector3(Math::RandFloatMinMax(-50, 50), 0, Math::RandFloatMinMax(-50, 50))));
		objBuilder.createObject(new Ring(this, Vector3(Math::RandFloatMinMax(-50, 50), 0, Math::RandFloatMinMax(-50, 50))));

	//objBuilder.createObject(new Door(this, Vector3(0, 0, -15)));


}

void SceneTutorial::Update(double dt) {

	_dt = (float)dt;
	_elapsedTime += _dt;

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
	//	if (camera.position.x-(torusposition.x)<=5)
	//	{
	//		ring = false;
	///*		ring2 = false;
	//		ring3 = false;
	//		ring4 = false;
	//		ring5 = false;*/
	//		count++;
	//	}

	camera.Update(dt);
	charManager.getAnimator().getAnimMetabee().ProcessAnimation((float)dt);
	objBuilder.objInteractor.updateInteraction();

	static bool canPress = true;

	if (!Application::IsKeyPressed('Q'))
		canPress = true;

	// Light on
	if (canPress && Application::IsKeyPressed('Q')) {
		light[0].power = (light[0].power > 0) ? 0.0f : 2.0f; // Toggle Power between 0 and 2
		glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
		canPress = false;
	}

	std::ostringstream strFlashlight;
	strFlashlight << "[Q] Flashlight: " << ((light[0].power) ? "On" : "Off");
	textManager.queueRenderText(UIManager::Text(strFlashlight.str(), Color(1, 1, 1), UIManager::ANCHOR_BOT_RIGHT));

	std::ostringstream strHealth;
	strHealth << "Health: " << charManager.getHealth();

	textManager.queueRenderText(UIManager::Text(strHealth.str(), (charManager.getHealth() <= 50) ? Color(1, 0, 0) : Color(0, 1, 0), UIManager::ANCHOR_BOT_LEFT));

	// Transforms for the hand animation when shooting
	float* shoulderRotX = &charManager.getAnimator().getAnimMetabee().transform_rotate_list[AnimMetabee::MTB_TRANSFORM_ROT::ROT_RightShoulderX];
	float* shoulderRotY = &charManager.getAnimator().getAnimMetabee().transform_rotate_list[AnimMetabee::MTB_TRANSFORM_ROT::ROT_RightShoulderY];
	float* armRotX = &charManager.getAnimator().getAnimMetabee().transform_rotate_list[AnimMetabee::MTB_TRANSFORM_ROT::ROT_RightArmX];
	float* armRotY = &charManager.getAnimator().getAnimMetabee().transform_rotate_list[AnimMetabee::MTB_TRANSFORM_ROT::ROT_RightArmY];



	static bool canGodmodePress = true;

	// Godmode key
	if (!Application::IsKeyPressed('C')) {
		canGodmodePress = true;
	}

	if (Application::IsKeyPressed('C') && canGodmodePress) {
		charManager.setGodMode(!charManager.isGodmodeEnabled());
		canGodmodePress = false;
	}

	// Flashlight position and direction
	light[0].position.Set(camera.position.x, camera.position.y, camera.position.z);
	light[0].spotDirection = camera.position - camera.target;

}

void SceneTutorial::Render() {
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z);
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

	RenderMesh(meshList[GEO_AXES], false);


	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_FLOOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(20, 4, -10);
	modelStack.Rotate(30, 0, 1, 0);
	modelStack.Scale(.8f, .8f, .8f);
	RenderMesh(meshList[GEO_SPACESHIP], true);
	modelStack.PopMatrix();
	if (ring == true)
	{
		modelStack.PushMatrix();
		modelStack.Translate(20, 10, -10);
		modelStack.Rotate(90, 0, 0, 1);
		modelStack.Scale(.5f, .5f, .5f);
		RenderMesh(meshList[GEO_RING], true);
		modelStack.PopMatrix();
	}
	if (ring == false)
	{
		textManager.renderTextOnScreen(UIManager::Text("you got a ring", Color(0, 1, 0), UIManager::ANCHOR_TOP_LEFT));
	}

	// Lamp
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 10);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_LAMP], true);

	{

		modelStack.PushMatrix();
		modelStack.Translate(0, 0, -20);
		RenderMesh(meshList[GEO_LAMP], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 0, -10);
		RenderMesh(meshList[GEO_LAMP], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 0, 0);
		RenderMesh(meshList[GEO_LAMP], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 0, 10);
		RenderMesh(meshList[GEO_LAMP], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 0, 20);
		RenderMesh(meshList[GEO_LAMP], true);
		modelStack.PopMatrix();

	}

	modelStack.PopMatrix();

	// Building
	modelStack.PushMatrix();
	{
		glDisable(GL_CULL_FACE); // Stop culling temporarily
		modelStack.Translate(0, 0, -15);
		modelStack.Scale(0.5f, 0.5f, 0.5f);
		RenderMesh(meshList[GEO_BUILDING], true);
		glEnable(GL_CULL_FACE); // Restore culling

		// Table & Chairs
		{
			modelStack.PushMatrix();
			modelStack.Scale(0.1f, 0.1f, 0.1f);
			modelStack.Rotate(90, 0, 1, 0);
			RenderMesh(meshList[GEO_TABLE], true);
			{
				modelStack.PushMatrix();
				modelStack.Translate(-20, 0, 0);
				modelStack.Scale(25, 25, 25);
				RenderMesh(meshList[GEO_CHAIR], true);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(20, 0, 0);
				modelStack.Scale(25, 25, 25);
				RenderMesh(meshList[GEO_CHAIR], true);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(0, 0, -35);
				modelStack.Scale(25, 25, 25);
				RenderMesh(meshList[GEO_CHAIR], true);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(0, 0, 35);
				modelStack.Scale(25, 25, 25);
				RenderMesh(meshList[GEO_CHAIR], true);
				modelStack.PopMatrix();
			}
			modelStack.PopMatrix();
		}

		// Crates
		{
			modelStack.PushMatrix();
			modelStack.Translate(8, 0, 0);
			modelStack.Scale(0.4f, 0.4f, 0.4f);
			RenderMesh(meshList[GEO_CRATE], true);

			{
				modelStack.PushMatrix();
				modelStack.Translate(0, 0, -5);
				modelStack.Rotate(60, 0, 1, 0);
				RenderMesh(meshList[GEO_CRATE], true);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(0, 0, 5);
				modelStack.Rotate(-45, 0, 1, 0);
				RenderMesh(meshList[GEO_CRATE], true);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(0, 4, 0);
				modelStack.Rotate(45, 0, 1, 0);
				RenderMesh(meshList[GEO_CRATE], true);
				modelStack.PopMatrix();
			}

			modelStack.PopMatrix();
		}

	}
	modelStack.PopMatrix();


	// Character Transform
	modelStack.PushMatrix();
	modelStack.Translate(camera.target.x, camera.target.y - 0.5f, camera.target.z);
	modelStack.Rotate(-camera.getYaw(), 0, 1, 0);
	modelStack.Rotate(-camera.getPitch(), 0, 0, 1);
	modelStack.Rotate(-camera.getRoll(), 1, 0, 0);
	modelStack.Scale(0.1f, 0.1f, 0.1f);
	RenderMesh(meshList[GEO_SPACESHIP], true);
	modelStack.PopMatrix();


	objBuilder.renderObjects();


	// Debugging Text
	std::ostringstream fps;
	fps << "FPS: " << (1 / _dt);
	textManager.renderTextOnScreen(UIManager::Text(fps.str(), Color(0, 1, 0), UIManager::ANCHOR_TOP_LEFT));
	std::ostringstream targ;
	targ << "Target: " << camera.getTarget().toString();
	textManager.renderTextOnScreen(UIManager::Text(targ.str(), Color(0, 1, 0), UIManager::ANCHOR_TOP_LEFT));
	std::ostringstream upz;
	upz << "Up: " << camera.getUp().toString();
	textManager.renderTextOnScreen(UIManager::Text(upz.str(), Color(0, 1, 0), UIManager::ANCHOR_TOP_LEFT));
	std::ostringstream pitch;
	pitch << "Pitch: " << camera.getPitch();
	textManager.renderTextOnScreen(UIManager::Text(pitch.str(), Color(0, 1, 0), UIManager::ANCHOR_TOP_LEFT));
	std::ostringstream yaw;
	yaw << "Yaw: " << camera.getYaw();
	textManager.renderTextOnScreen(UIManager::Text(yaw.str(), Color(0, 1, 0), UIManager::ANCHOR_TOP_LEFT));

	// Crosshair
	textManager.renderTextOnScreen(UIManager::Text("+", Color(0, 1, 0), UIManager::ANCHOR_CENTER_CENTER));

	textManager.renderTextOnScreen(UIManager::Text("<Objective>", Color(1, 1, 1), UIManager::ANCHOR_TOP_CENTER));
	textManager.renderTextOnScreen(UIManager::Text("Collect [6] Rings", Color(1, 1, 1), UIManager::ANCHOR_TOP_CENTER));


	std::ostringstream score;
	score << "Rocks remaining: " << (10 - charManager.getCurrentScore());

	textManager.renderTextOnScreen(UIManager::Text(score.str(), Color(0, 1, 0), UIManager::ANCHOR_TOP_RIGHT));

	std::ostringstream slimes;
	slimes << "Slimes Remaining: " << Slime::slimeCount;

	textManager.renderTextOnScreen(UIManager::Text(slimes.str(), (Slime::slimeCount <= 0) ? Color(1, 1, 1) : Color(0, 1, 0), UIManager::ANCHOR_TOP_RIGHT));

	std::ostringstream godmode;
	godmode << "Godmode: Enabled";

	if (charManager.isGodmodeEnabled())
		textManager.renderTextOnScreen(UIManager::Text(godmode.str(), Color(0, 0, 1), UIManager::ANCHOR_CENTER_CENTER));


	textManager.dequeueText();

	RenderMeshOnScreen(meshList[GEO_CRATE], 5, 5, 1, 1);

	textManager.reset();
}

void SceneTutorial::RenderMesh(Mesh *mesh, bool enableLight) {
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

void SceneTutorial::RenderMeshOnScreen(Mesh* mesh, int x, int y, float sizex, float sizey) {
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, Application::_windowWidth / 10, 0, Application::_windowHeight / 10, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate((float)x, (float)y, 1);
	modelStack.Scale(sizex, sizey, 1);
	RenderMesh(mesh, false); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneTutorial::RenderSkybox() {

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

void SceneTutorial::RenderCharacter() {

	// Metabee
	charManager.getAnimator().getAnimMetabee().popChest();
	charManager.getRenderer().renderMtbChest(charManager.getAnimator().getAnimMetabee());

	// Head
	/*
	modelStack.PushMatrix();
	modelStack.Translate(0, 2.25f, 0);

	charManager.getAnimator().getAnimMetabee().popHead();
	charManager.getRenderer().renderMtbHead();
	modelStack.PopMatrix();
	*/

	// Abdomen
	modelStack.PushMatrix();
	modelStack.Translate(0, -1.75f, 0);
	charManager.getAnimator().getAnimMetabee().popAbdomen();
	charManager.getRenderer().renderMtbAbdomen();
	modelStack.PopMatrix();

	// Left Arm
	modelStack.PushMatrix();
	modelStack.Translate(-1.75f, 0.5f, 0);
	charManager.getAnimator().getAnimMetabee().popLeftShoulder();
	charManager.getRenderer().renderMtbLeftShoulder(charManager.getAnimator().getAnimMetabee());

	modelStack.PushMatrix();
	modelStack.Translate(0, -1.25f, 0);
	charManager.getAnimator().getAnimMetabee().popLeftArm();
	charManager.getRenderer().renderMtbLeftArm();

	// Bullets
	modelStack.PushMatrix();
	charManager.getAnimator().getAnimMetabee().popLeftBullet();
	charManager.getRenderer().renderMtbLeftBullet();
	modelStack.PopMatrix();

	// Sword
	modelStack.PushMatrix();
	modelStack.Translate(0.25f, -1.75f, 0.25f);
	modelStack.Rotate(90, 1, 0, 0);
	charManager.getAnimator().getAnimMetabee().popSword();
	if (charManager.getAnimator().getAnimMetabee().isSwordOut()) { // Only render sword if it's "out"
		charManager.getRenderer().renderMtbSword();
	}
	modelStack.PopMatrix();

	modelStack.PopMatrix();
	modelStack.PopMatrix();

	// Left Leg
	modelStack.PushMatrix();
	modelStack.Translate(-0.5f, -2.25f, 0);
	charManager.getAnimator().getAnimMetabee().popLeftThigh();
	charManager.getRenderer().renderMtbLeftThigh();

	modelStack.PushMatrix();
	modelStack.Translate(0, -1.25f, 0);
	charManager.getAnimator().getAnimMetabee().popLeftFoot();
	charManager.getRenderer().renderMtbLeftFoot();
	modelStack.PopMatrix();

	modelStack.PopMatrix();

	// Right Leg
	modelStack.PushMatrix();
	modelStack.Translate(0.5f, -2.25f, 0);
	charManager.getAnimator().getAnimMetabee().popRightThigh();
	charManager.getRenderer().renderMtbLeftThigh();

	modelStack.PushMatrix();
	modelStack.Translate(0, -1.25f, 0);
	charManager.getAnimator().getAnimMetabee().popRightFoot();
	charManager.getRenderer().renderMtbLeftFoot();
	modelStack.PopMatrix();

	modelStack.PopMatrix();

	// Right Arm
	modelStack.PushMatrix();
	modelStack.Translate(1.75f, 0.5f, 0);
	modelStack.Mirror(1, 0, 0); // Flip the Arm
	glCullFace(GL_FRONT); // Stop culling temporarily
	charManager.getAnimator().getAnimMetabee().popRightShoulder();
	charManager.getRenderer().renderMtbLeftShoulder(charManager.getAnimator().getAnimMetabee());


	modelStack.PushMatrix();
	modelStack.Translate(0, -1.25f, 0);
	charManager.getAnimator().getAnimMetabee().popRightArm();
	charManager.getRenderer().renderMtbLeftArm();
	glCullFace(GL_BACK); // restore culling
	modelStack.PopMatrix();
	modelStack.PopMatrix();

}

void SceneTutorial::Exit() {

	for (int i = 0; i < NUM_GEOMETRY; ++i) {
		if (meshList[i] != nullptr) {
			delete meshList[i];
		}
	}

	objBuilder.objInteractor.~ObjectInteractor();
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
