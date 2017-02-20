#include "Scene.h"

#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "SceneTutorial.h"
#include "SceneMainMenu.h"
#include "SceneShop.h"
#include "SceneGameOver.h"


Scene::Scene(TYPE_SCENE type) :
pauseManager(this),
camera(this),
waypoint(this),
textManager(this),
skillManager(this), sceneType(type) {
}


Scene* Scene::createScene(TYPE_SCENE type) {


	switch (type)
	{
	case Scene::SCENE_TUTORIAL:
		return new SceneTutorial();
	case Scene::SCENE_MAINMENU:
		return new SceneMainMenu();
	case Scene::SCENE_SHOP:
		return new SceneShop();
	case Scene::SCENE_DOGFIGHT:
	case Scene::SCENE_CARGOSHIP:
	case Scene::SCENE_BOSS:
		break;
	}

	return nullptr;

}

void Scene::RenderMesh(Mesh *mesh, bool enableLight) {
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