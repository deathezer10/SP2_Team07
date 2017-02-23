#include "Application.h"
#include "NPC.h"
#include "Scene.h"

#include "GL\glew.h"

NPC::NPC(Scene* scene, Vector3 pos, bool showOnRadar) : Object(scene, pos){

	_isInsideRadar = showOnRadar;

	if (showOnRadar)
		scene->textManager.radar.addUnit(this);
};

NPC::~NPC(){
	if (_isInsideRadar)
		_scene->textManager.radar.removeUnit(this);
};

void NPC::render() {
	_scene->modelStack.PushMatrix();
	_scene->modelStack.Translate(position.x, position.y, position.z);
	_scene->modelStack.Rotate(rotationY, 0, 1, 0);
	_scene->modelStack.Rotate(rotationZ, 0, 0, 1);
	_scene->modelStack.Rotate(rotationX, 1, 0, 0);
	_scene->modelStack.Scale(scale, scale, scale);
	_scene->RenderMesh(_scene->meshList[type], isLightingEnabled);

	if (_isHealthBarEnabled){

		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		const float overallSize = 0.5f;
		float barSize = ((float)currentHP / (float)defaultHP) * overallSize;
		
		_scene->modelStack.PushMatrix();
		{
			_scene->modelStack.Translate(0, 0.25f, -overallSize);
			_scene->modelStack.Rotate(-90, 0, 1, 0);
			_scene->modelStack.Scale(overallSize, 0.025f, 0.1f);
			_scene->RenderMesh(_scene->meshList[Scene::GEO_HP_BACKGROUND], isLightingEnabled);
		}
		_scene->modelStack.PopMatrix();

		_scene->modelStack.PushMatrix();
		{
			_scene->modelStack.Translate(0, 0.25f, -barSize);
			_scene->modelStack.Rotate(-90, 0, 1, 0);
			_scene->modelStack.Scale(barSize, 0.025f, 0.1f);
			_scene->RenderMesh(_scene->meshList[Scene::GEO_HP_FOREGROUND], isLightingEnabled);
		}
		_scene->modelStack.PopMatrix();
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

	}

	_scene->modelStack.PopMatrix();


}

void NPC::setHealth(int value) {
	currentHP = defaultHP = value;
}

void NPC::reduceHealth(int value) {

	if (value <= 0)
		return;

	currentHP -= value;
	currentHP = Math::Clamp(currentHP, 0, 500);

}


void NPC::setVelocity(float value) {
	_currentVelocity = value;
}


void NPC::reduceVelocity(float value) {
	_currentVelocity -= value;
}