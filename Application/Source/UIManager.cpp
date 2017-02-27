#include <fstream>

#include "Application.h"

#include "GL\glew.h"
#include "GLFW\glfw3.h"

#include "Scene.h"
#include "Mesh.h"
#include "UIManager.h"
#include "PlayerDataManager.h"

#include <sstream>


using std::ostringstream;


bool UIManager::showDebugInfo = false;


UIManager::UIManager(Scene* scene) : radar(scene) {

	_scene = scene;

	textMesh = &(_scene->meshList[Scene::GEO_TEXT]);

	anchor_offset[ANCHOR_BOT_LEFT] = 0;
	anchor_offset[ANCHOR_BOT_RIGHT] = 0;
	anchor_offset[ANCHOR_TOP_LEFT] = (Application::windowHeight() / 10) - 2;
	anchor_offset[ANCHOR_TOP_RIGHT] = (Application::windowHeight() / 10) - 2;
	anchor_offset[ANCHOR_BOT_CENTER] = 0;
	anchor_offset[ANCHOR_TOP_CENTER] = (Application::windowHeight() / 10) - 2;
	anchor_offset[ANCHOR_CENTER_CENTER] = 0;

}

bool UIManager::LoadFontWidth(std::string fontPath) {

	// Clear vector if it's loaded
	if (currentFontWidth.empty())
		currentFontWidth.clear();

	std::ifstream fileStream(fontPath);

	if (!fileStream.is_open()) {
		std::cout << "Impossible to open " << fontPath << ". Are you in the right directory ?\n";
		return false;
	}

	std::string data;
	while (std::getline(fileStream, data, '\n')) // read every line
	{
		currentFontWidth.push_back(std::stoul(data));
	}

	return true;
}

//Render the mesh onto the world
void UIManager::queueRenderMesh(MeshQueue meshQueue) {
	currentMeshQueue.push(meshQueue);
}

void UIManager::dequeueMesh() {

	// Render all queued meshes
	while (!currentMeshQueue.empty()) {

		MeshQueue mesh = currentMeshQueue.front();

		_scene->modelStack.PushMatrix();
		_scene->modelStack.Translate(mesh.position.x, mesh.position.y, mesh.position.z);
		_scene->modelStack.Scale(mesh.scaling.x, mesh.scaling.y, mesh.scaling.z);
		_scene->modelStack.Rotate(mesh.rotation.x, 1, 0, 0);
		_scene->modelStack.Rotate(mesh.rotation.y, 0, 1, 0);
		_scene->modelStack.Rotate(mesh.rotation.z, 0, 0, 1);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //default fill mode
		_scene->RenderMesh(mesh.mesh, mesh.lighting);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
		_scene->modelStack.PopMatrix();

		currentMeshQueue.pop();
	}
}

void UIManager::queueRenderText(Text text) {
	currentTextQueue.push(text);
}

void UIManager::dequeueText() {
	// Print all the queued Texts
	while (!currentTextQueue.empty()) {
		renderTextOnScreen(currentTextQueue.front());
		currentTextQueue.pop();
	}
}

void UIManager::renderTextOnScreen(Text text) {

	if (!textMesh || (*textMesh)->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);

	Mtx44 ortho;
	ortho.SetToOrtho(0, Application::windowWidth() / 10, 0, Application::windowHeight() / 10, -10, 10); //size of screen UI
	_scene->projectionStack.PushMatrix();
	_scene->projectionStack.LoadMatrix(ortho);
	_scene->viewStack.PushMatrix();
	_scene->viewStack.LoadIdentity(); //No need camera for ortho mode
	_scene->modelStack.PushMatrix();
	_scene->modelStack.LoadIdentity(); //Reset modelStack

	glUniform1i(_scene->m_parameters[Scene::U_TEXT_ENABLED], 1);
	glUniform3fv(_scene->m_parameters[Scene::U_TEXT_COLOR], 1, &text.color.r);
	glUniform1i(_scene->m_parameters[Scene::U_LIGHTENABLED], 0);
	glUniform1i(_scene->m_parameters[Scene::U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, (*textMesh)->textureID);
	glUniform1i(_scene->m_parameters[Scene::U_COLOR_TEXTURE], 0);

	// Width of the entire string
	float totalWidth = 0;

	// Calculate total width of the string, used for offsetting the anchors
	for (unsigned i = 0; i < text.value.length(); ++i) {
		totalWidth += currentFontWidth[text.value[i]] / 32.0f;
	}

	float tX = 0, tY = 0;

	// Move the text position according to the given anchor
	switch (text.anchor) {

	case ANCHOR_BOT_LEFT:
		tY = (float)anchor_offset[ANCHOR_BOT_LEFT];
		anchor_offset[ANCHOR_BOT_LEFT] += 2;
		break;

	case ANCHOR_BOT_RIGHT:
		tX = (Application::windowWidth() / 10) - totalWidth;
		tY = (float)anchor_offset[ANCHOR_BOT_RIGHT];
		anchor_offset[ANCHOR_BOT_RIGHT] += 2;
		break;

	case ANCHOR_TOP_LEFT:
		tY = (float)anchor_offset[ANCHOR_TOP_LEFT];
		anchor_offset[ANCHOR_TOP_LEFT] -= 2;
		break;

	case ANCHOR_TOP_RIGHT:
		tX = (Application::windowWidth() / 10) - totalWidth;
		tY = (float)anchor_offset[ANCHOR_TOP_RIGHT];
		anchor_offset[ANCHOR_TOP_RIGHT] -= 2;
		break;

	case ANCHOR_BOT_CENTER:
		tX = (Application::windowWidth() / 20) - (totalWidth / 2);
		tY = (float)anchor_offset[ANCHOR_BOT_CENTER];
		anchor_offset[ANCHOR_BOT_CENTER] += 2;
		break;

	case ANCHOR_TOP_CENTER:
		tX = (Application::windowWidth() / 20) - (totalWidth / 2);
		tY = (float)anchor_offset[ANCHOR_TOP_CENTER];
		anchor_offset[ANCHOR_TOP_CENTER] -= 2;
		break;

	case ANCHOR_CENTER_CENTER:
		tX = (Application::windowWidth() / 20) - (totalWidth / 2);
		tY = (Application::windowHeight() / 20) - (float)anchor_offset[ANCHOR_CENTER_CENTER];
		anchor_offset[ANCHOR_CENTER_CENTER] += 2;
		break;

	default:
		break;

	}

	_scene->modelStack.Translate(tX + 1, tY + 1, 0); // minor offset to display properly

	MS textStack;
	unsigned stackCount = 0;

	float prevWidth = 0; // the previous printed character width

	for (unsigned i = 0; i < text.value.length(); ++i, ++stackCount) {

		textStack.PushMatrix();
		textStack.Translate(prevWidth, 0, 0); // spacing of each character

		prevWidth = currentFontWidth[text.value[i]] / 32.0f;

		Mtx44 MVP = _scene->projectionStack.Top() * _scene->viewStack.Top() * _scene->modelStack.Top() * textStack.Top();
		glUniformMatrix4fv(_scene->m_parameters[Scene::U_MVP], 1, GL_FALSE, &MVP.a[0]);

		(*textMesh)->Render((unsigned)text.value[i] * 6, 6);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(_scene->m_parameters[Scene::U_TEXT_ENABLED], 0);

	for (unsigned i = 0; i < stackCount; ++i) {
		textStack.PopMatrix();
	}

	_scene->projectionStack.PopMatrix();
	_scene->viewStack.PopMatrix();
	_scene->modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}

void UIManager::renderPlayerHUD() {

	static bool canDebugPress = false;

	if (!Application::IsKeyPressed('C')) {
		canDebugPress = true;
	}

	if (Application::IsKeyPressed('C') && canDebugPress) {
		canDebugPress = false;
		showDebugInfo = !showDebugInfo;
	}

	// Display debugging information
	if (showDebugInfo) {

		std::ostringstream fps;
		fps << "FPS: " << (int)(1 / _scene->_dt);
		renderTextOnScreen(UIManager::Text(fps.str(), Color(0, 1, 0), UIManager::ANCHOR_TOP_LEFT));
		std::ostringstream targ;
		targ << "Target: " << _scene->camera.getTarget().toString();
		renderTextOnScreen(UIManager::Text(targ.str(), Color(0, 1, 0), UIManager::ANCHOR_TOP_LEFT));
		std::ostringstream upz;
		upz << "Up: " << _scene->camera.getUp().toString();
		renderTextOnScreen(UIManager::Text(upz.str(), Color(0, 1, 0), UIManager::ANCHOR_TOP_LEFT));
		std::ostringstream pitch;
		pitch << "Pitch: " << _scene->camera.getPitch();
		renderTextOnScreen(UIManager::Text(pitch.str(), Color(0, 1, 0), UIManager::ANCHOR_TOP_LEFT));
		std::ostringstream yaw;
		yaw << "Yaw: " << _scene->camera.getYaw();
		renderTextOnScreen(UIManager::Text(yaw.str(), Color(0, 1, 0), UIManager::ANCHOR_TOP_LEFT));

		Collider collider = _scene->camera.getCollider();

		queueRenderMesh(UIManager::MeshQueue{

			_scene->meshList[Scene::GEO_CUBE],
			collider.getPosition(),
			Vector3(0, 0, 0),
			Vector3(collider.bboxWidth, collider.bboxHeight, collider.bboxDepth)

		});
	}

	std::ostringstream strHealth;
	strHealth << "Health: " << PlayerDataManager::getInstance()->getPlayerStats()->current_health;
	renderTextOnScreen(UIManager::Text(strHealth.str(), (PlayerDataManager::getInstance()->getPlayerStats()->current_health <= 50) ? Color(1, 0, 0) : Color(0, 1, 0), UIManager::ANCHOR_BOT_CENTER));

	std::ostringstream strShield;
	strShield << "Shield: " << (int)PlayerDataManager::getInstance()->getPlayerStats()->current_shield;
	renderTextOnScreen(UIManager::Text(strShield.str(), (PlayerDataManager::getInstance()->getPlayerStats()->current_shield <= 50) ? Color(1, 0, 0) : Color(0, 1, 1), UIManager::ANCHOR_BOT_CENTER));

	float healthScale = (float)PlayerDataManager::getInstance()->getPlayerStats()->current_health / 100;
	float shieldScale = (float)PlayerDataManager::getInstance()->getPlayerStats()->current_shield / (float)PlayerDataManager::getInstance()->getPlayerStats()->current_shield_capacity;

	float winHeight = (float)Application::windowHeight() / 10;
	float winWidth = (float)Application::windowWidth() / 10;

	// Health Bar
	RenderMeshOnScreen(_scene->meshList[Scene::GEO_HP_FOREGROUND], winWidth * 0.25f, winHeight * 0.01f, Vector3(0, 0, 90), Vector3(10 * healthScale, 1, 1));
	RenderMeshOnScreen(_scene->meshList[Scene::GEO_HP_BACKGROUND], winWidth * 0.25f, winHeight * 0.01f, Vector3(0, 0, 90), Vector3(10, 1, 1));

	// Shield Bar
	RenderMeshOnScreen(_scene->meshList[Scene::GEO_SHIELD_FOREGROUND], winWidth * 0.75f, winHeight * 0.01f, Vector3(0, 0, 90), Vector3(10 * shieldScale, 1, 1));
	RenderMeshOnScreen(_scene->meshList[Scene::GEO_HP_BACKGROUND], winWidth * 0.75f, winHeight * 0.01f, Vector3(0, 0, 90), Vector3(10, 1, 1));

	// Speed
	std::ostringstream velocity;
	velocity << "Speed: " << (int)_scene->camera.getCurrentVelocity() << "m/s";
	renderTextOnScreen(UIManager::Text(velocity.str(), Color(1, 1, 1), UIManager::ANCHOR_BOT_RIGHT));

	// Crosshair
	renderTextOnScreen(UIManager::Text("", Color(0, 0, 0), UIManager::ANCHOR_CENTER_CENTER));
	renderTextOnScreen(UIManager::Text("+", Color(0, 1, 0), UIManager::ANCHOR_CENTER_CENTER));

	// Radar
	radar.RenderRadar(winWidth * 0.075f, winHeight * 0.125f);

	RenderTimedMeshOnScreen();
}


void UIManager::addTimedMeshToScreen(MeshQueue mesh, float duration){
	currentTimeMeshes[mesh] = duration;
}


void UIManager::RenderTimedMeshOnScreen(){


	for (auto &it = currentTimeMeshes.begin(); it != currentTimeMeshes.end();){

		MeshQueue mq = it->first;

		if (it->second > 0){
			RenderMeshOnScreen(mq.mesh, mq.position.x, mq.position.y, mq.rotation, mq.scaling);
			it->second -= _scene->_dt;
			++it;
		}
		else {
			it = currentTimeMeshes.erase(it);
		}
	}

}

void UIManager::RenderMeshOnScreen(Mesh* mesh, float x, float y, Vector3 rotate, Vector3 scale) {
	// glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, Application::windowWidth() / 10, 0, Application::windowHeight() / 10, -100, 100); //size of screen UI
	_scene->projectionStack.PushMatrix();
	_scene->projectionStack.LoadMatrix(ortho);
	_scene->viewStack.PushMatrix();
	_scene->viewStack.LoadIdentity(); //No need camera for ortho mode
	_scene->modelStack.PushMatrix();
	_scene->modelStack.LoadIdentity();
	_scene->modelStack.Translate(x, y, 1);
	_scene->modelStack.Rotate(rotate.y, 0, 1, 0);
	_scene->modelStack.Rotate(rotate.z, 0, 0, 1);
	_scene->modelStack.Rotate(rotate.x, 1, 0, 0);
	_scene->modelStack.Scale(scale.x, scale.y, scale.z);
	_scene->RenderMesh(mesh, false); //UI should not have light
	_scene->projectionStack.PopMatrix();
	_scene->viewStack.PopMatrix();
	_scene->modelStack.PopMatrix();
	// glEnable(GL_DEPTH_TEST);
}

void UIManager::reset() {

	anchor_offset[ANCHOR_BOT_LEFT] = 0;
	anchor_offset[ANCHOR_BOT_RIGHT] = 0;
	anchor_offset[ANCHOR_TOP_LEFT] = (Application::windowHeight() / 10) - 2;
	anchor_offset[ANCHOR_TOP_RIGHT] = (Application::windowHeight() / 10) - 2;
	anchor_offset[ANCHOR_BOT_CENTER] = 0;
	anchor_offset[ANCHOR_TOP_CENTER] = (Application::windowHeight() / 10) - 2;
	anchor_offset[ANCHOR_CENTER_CENTER] = 0;

}