#include <fstream>

#include "Application.h"

#include "GL\glew.h"
#include "GLFW\glfw3.h"

#include "Scene.h"
#include "UIManager.h"
#include "PlayerDataManager.h"

#include <sstream>

using std::ostringstream;


UIManager::UIManager(Scene* scene) {

	_scene = scene;

	textMesh = &(_scene->meshList[Scene::GEO_TEXT]);

	anchor_offset[ANCHOR_BOT_LEFT] = 0;
	anchor_offset[ANCHOR_BOT_RIGHT] = 0;
	anchor_offset[ANCHOR_TOP_LEFT] = (Application::_windowHeight / 10) - 2;
	anchor_offset[ANCHOR_TOP_RIGHT] = (Application::_windowHeight / 10) - 2;
	anchor_offset[ANCHOR_BOT_CENTER] = 0;
	anchor_offset[ANCHOR_TOP_CENTER] = (Application::_windowHeight / 10) - 2;
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
	ortho.SetToOrtho(0, Application::_windowWidth / 10, 0, Application::_windowHeight / 10, -10, 10); //size of screen UI
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
		tX = (Application::_windowWidth / 10) - totalWidth;
		tY = (float)anchor_offset[ANCHOR_BOT_RIGHT];
		anchor_offset[ANCHOR_BOT_RIGHT] += 2;
		break;

	case ANCHOR_TOP_LEFT:
		tY = (float)anchor_offset[ANCHOR_TOP_LEFT];
		anchor_offset[ANCHOR_TOP_LEFT] -= 2;
		break;

	case ANCHOR_TOP_RIGHT:
		tX = (Application::_windowWidth / 10) - totalWidth;
		tY = (float)anchor_offset[ANCHOR_TOP_RIGHT];
		anchor_offset[ANCHOR_TOP_RIGHT] -= 2;
		break;

	case ANCHOR_BOT_CENTER:
		tX = (Application::_windowWidth / 20) - (totalWidth / 2);
		tY = (float)anchor_offset[ANCHOR_BOT_CENTER];
		anchor_offset[ANCHOR_BOT_CENTER] += 2;
		break;

	case ANCHOR_TOP_CENTER:
		tX = (Application::_windowWidth / 20) - (totalWidth / 2);
		tY = (float)anchor_offset[ANCHOR_TOP_CENTER];
		anchor_offset[ANCHOR_TOP_CENTER] -= 2;
		break;

	case ANCHOR_CENTER_CENTER:
		tX = (Application::_windowWidth / 20) - (totalWidth / 2);
		tY = (Application::_windowHeight / 20) - (float)anchor_offset[ANCHOR_CENTER_CENTER];
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

void UIManager::renderPlayerHUD(){

	static bool canDebugPress = false;

	if (!Application::IsKeyPressed('C')) {
		canDebugPress = true;
	}

	if (Application::IsKeyPressed('C') && canDebugPress) {
		canDebugPress = false;
		showDebugInfo = !showDebugInfo;
	}

	if (showDebugInfo) {
		// Debugging Text
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
	}

	std::ostringstream strHealth;
	strHealth << "Health: " << PlayerDataManager::getInstance()->getPlayerStats()->current_health;
	renderTextOnScreen(UIManager::Text(strHealth.str(), (PlayerDataManager::getInstance()->getPlayerStats()->current_health <= 50) ? Color(1, 0, 0) : Color(0, 1, 0), UIManager::ANCHOR_BOT_LEFT));

	std::ostringstream strShield;
	strShield << "Shield: " << (int)PlayerDataManager::getInstance()->getPlayerStats()->current_shield;
	renderTextOnScreen(UIManager::Text(strShield.str(), (PlayerDataManager::getInstance()->getPlayerStats()->current_shield <= 50) ? Color(1, 0, 0) : Color(.31f, .81f, .99f), UIManager::ANCHOR_BOT_LEFT));


	std::ostringstream velocity;
	velocity << "Speed: " << (int)_scene->camera.getCurrentVelocity() << "m/s";
	renderTextOnScreen(UIManager::Text(velocity.str(), Color(1, 1, 1), UIManager::ANCHOR_BOT_RIGHT));

	// Crosshair
	renderTextOnScreen(UIManager::Text("+", Color(0, 1, 0), UIManager::ANCHOR_CENTER_CENTER));
}

void UIManager::RenderMeshOnScreen(Mesh* mesh, int x, int y, Vector3 rotate, Vector3 scale) {
	// glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, Application::_windowWidth / 10, 0, Application::_windowHeight / 10, -100, 100); //size of screen UI
	_scene->projectionStack.PushMatrix();
	_scene->projectionStack.LoadMatrix(ortho);
	_scene->viewStack.PushMatrix();
	_scene->viewStack.LoadIdentity(); //No need camera for ortho mode
	_scene->modelStack.PushMatrix();
	_scene->modelStack.LoadIdentity();
	_scene->modelStack.Translate((float)x, (float)y, 1);
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
	anchor_offset[ANCHOR_TOP_LEFT] = (Application::_windowHeight / 10) - 2;
	anchor_offset[ANCHOR_TOP_RIGHT] = (Application::_windowHeight / 10) - 2;
	anchor_offset[ANCHOR_BOT_CENTER] = 0;
	anchor_offset[ANCHOR_TOP_CENTER] = (Application::_windowHeight / 10) - 2;
	anchor_offset[ANCHOR_CENTER_CENTER] = 0;

}