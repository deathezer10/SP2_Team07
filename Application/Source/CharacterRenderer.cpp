#include "GL\glew.h"
#include "MeshBuilder.h"
#include "Mtx44.h"

#include "CharacterRenderer.h"
#include "Scene.h"


CharacterRenderer::CharacterRenderer(Scene* scene)
{
	_scene = scene;

	meshList = _scene->meshList;
	modelStack = &_scene->modelStack;

	// Build Meshes before rendering
	prepareMtbHead();

	prepareMtbLeftShoulder();
	prepareMtbLeftArm();

	prepareMtbChest();
	prepareMtbAbdomen();

	prepareMtbLeftThigh();
	prepareMtbLeftFoot();

	prepareMtbSword();

}

CharacterRenderer::~CharacterRenderer() {
}

void CharacterRenderer::RenderMesh(Mesh *mesh, bool enableLight) {
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = _scene->projectionStack.Top() * _scene->viewStack.Top() * _scene->modelStack.Top();
	glUniformMatrix4fv(_scene->m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = _scene->viewStack.Top() * _scene->modelStack.Top();
	glUniformMatrix4fv(_scene->m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if (enableLight) {
		glUniform1i(_scene->m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(_scene->m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(_scene->m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(_scene->m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(_scene->m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(_scene->m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else {
		glUniform1i(_scene->m_parameters[U_LIGHTENABLED], 0);
	}

	if (mesh->textureID > 0) {
		glUniform1i(_scene->m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(_scene->m_parameters[U_COLOR_TEXTURE], 0);
	}
	else {
		glUniform1i(_scene->m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}

	mesh->Render(); //this line should only be called once

	if (mesh->textureID > 0) {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}

void CharacterRenderer::prepareMtbHead() {

	for (unsigned i = GEO_HEAD_CANNON; i <= GEO_HEAD_FACE_MOUTH; i++) {
		switch (i) {

		case GEO_HEAD_CANNON:
			meshList[i] = MeshBuilder::GenerateHollowCylinder("GEO_HEAD_CANNON", Color(.12f, .18f, .32f), 18, 1.0f, 0.4f, 0.15f);
			break;

		case GEO_HEAD_CONNECTOR_BRANCH:
			meshList[i] = MeshBuilder::GenerateCylinder("GEO_HEAD_CONNECTOR_BRANCH", Color(.65f, .46f, .11f), 15, 0.75f, 0.15f);
			break;

		case GEO_HEAD_CONNECTOR_BASE:
			meshList[i] = MeshBuilder::GenerateTriangleCuboid("GEO_HEAD_CONNECTOR_BASE", Color(.65f, .46f, .11f), 0.5f, -0.5f, 0.15f);
			break;

		case GEO_HEAD_SKULL_FRONT:
			meshList[i] = MeshBuilder::GenerateTriangleCuboid("GEO_HEAD_SKULL_FRONT", Color(.65f, .46f, .11f), 1, -0.5f, 1);
			break;

		case GEO_HEAD_SKULL_BACK:
			meshList[i] = MeshBuilder::GenerateCube("GEO_HEAD_SKULL_BACK", Color(.65f, .46f, .11f), 1, 1, .75f);
			break;

		case GEO_HEAD_FACE_EYE:
			meshList[i] = MeshBuilder::GenerateDiamondQuad("GEO_HEAD_FACE_EYE", Color(.15f, .63f, .18f), 0.4f, 0.25f);
			break;

		case GEO_HEAD_FACE_EYE_SHADOW:
			meshList[i] = MeshBuilder::GenerateQuad("GEO_HEAD_FACE_EYE_SHADOW", Color(.0f, .0f, .0f), 0.56f, 0.35f);
			break;

		case GEO_HEAD_FACE_MOUTH:
			meshList[i] = MeshBuilder::GenerateQuad("GEO_HEAD_FACE_MOUTH", Color(.45f, .56f, .58f), 0.57f, 0.35f);
			break;


		}
		meshList[i]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[i]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
		meshList[i]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
		meshList[i]->material.kShininess = 1.0f;
	}

}

void CharacterRenderer::renderMtbHead() {

	// Skull
	modelStack->PushMatrix();
	RenderMesh(meshList[GEO_HEAD_SKULL_BACK], true);
	modelStack->PopMatrix();

	modelStack->PushMatrix();
	modelStack->Translate(0, 0, 1.75f);
	modelStack->Rotate(90, 1, 0, 0);
	RenderMesh(meshList[GEO_HEAD_SKULL_FRONT], true);
	modelStack->PopMatrix();

	// Eye Shadow
	modelStack->PushMatrix();
	modelStack->Translate(-0.5f, 0.06f, 1.01f);
	modelStack->Rotate(-26, 0, 1, 0);
	modelStack->Rotate(90, 1, 0, 0);
	RenderMesh(meshList[GEO_HEAD_FACE_EYE_SHADOW], true);
	modelStack->PopMatrix();

	modelStack->PushMatrix();
	modelStack->Translate(0.5f, 0.06f, 1.01f);
	modelStack->Rotate(26, 0, 1, 0);
	modelStack->Rotate(90, 1, 0, 0);
	RenderMesh(meshList[GEO_HEAD_FACE_EYE_SHADOW], true);
	modelStack->PopMatrix();

	// Eyes
	modelStack->PushMatrix();
	modelStack->Translate(0.55f, 0.05f, 1.02f);
	modelStack->Rotate(25, 0, 1, 0);
	modelStack->Rotate(15, 0, 0, 1);
	RenderMesh(meshList[GEO_HEAD_FACE_EYE], true);
	modelStack->PopMatrix();

	modelStack->PushMatrix();
	modelStack->Translate(-0.55f, 0.05f, 1.02f);
	modelStack->Rotate(-25, 0, 1, 0);
	modelStack->Rotate(-15, 0, 0, 1);
	RenderMesh(meshList[GEO_HEAD_FACE_EYE], true);
	modelStack->PopMatrix();

	// Mouth
	modelStack->PushMatrix();
	modelStack->Translate(-0.5f, -0.65f, 1.01f);
	modelStack->Rotate(-26, 0, 1, 0);
	modelStack->Rotate(90, 1, 0, 0);
	RenderMesh(meshList[GEO_HEAD_FACE_MOUTH], true);
	modelStack->PopMatrix();

	modelStack->PushMatrix();
	modelStack->Translate(0.5f, -0.65f, 1.01f);
	modelStack->Rotate(26, 0, 1, 0);
	modelStack->Rotate(90, 1, 0, 0);
	RenderMesh(meshList[GEO_HEAD_FACE_MOUTH], true);
	modelStack->PopMatrix();

	// Head Connectors
	modelStack->PushMatrix();
	modelStack->Translate(0, 1.15f, 1.75f);
	modelStack->Rotate(90, 1, 0, 0);
	RenderMesh(meshList[GEO_HEAD_CONNECTOR_BASE], true);
	modelStack->PopMatrix();

	modelStack->PushMatrix();
	modelStack->Translate(0.25f, 1.25f, 1.25f);
	modelStack->Rotate(45, 0, 1, 0);
	modelStack->Rotate(60, 1, 0, 0);
	RenderMesh(meshList[GEO_HEAD_CONNECTOR_BRANCH], true);
	modelStack->PopMatrix();

	modelStack->PushMatrix();
	modelStack->Translate(-0.25f, 1.25f, 1.25f);
	modelStack->Rotate(-45, 0, 1, 0);
	modelStack->Rotate(60, 1, 0, 0);
	RenderMesh(meshList[GEO_HEAD_CONNECTOR_BRANCH], true);
	modelStack->PopMatrix();

	// Head Cannons
	modelStack->PushMatrix();
	modelStack->Translate(0.65f, 1.6f, 1.6f);
	modelStack->Rotate(45, 0, 1, 0);
	modelStack->Rotate(60, 1, 0, 0);
	RenderMesh(meshList[GEO_HEAD_CANNON], true);
	modelStack->PopMatrix();

	modelStack->PushMatrix();
	modelStack->Translate(-0.65f, 1.6f, 1.6f);
	modelStack->Rotate(-45, 0, 1, 0);
	modelStack->Rotate(60, 1, 0, 0);
	RenderMesh(meshList[GEO_HEAD_CANNON], true);
	modelStack->PopMatrix();


}

void CharacterRenderer::prepareMtbLeftShoulder() {
	for (unsigned i = GEO_SHOULDER_CANNON_BARREL; i <= GEO_SHOULDER_CHARGED_PARTICLE; i++) {

		switch (i) {

		case GEO_SHOULDER_CANNON_BARREL:
			meshList[i] = MeshBuilder::GenerateCylinder("GEO_SHOULDER_CANNON_BARREL", Color(.12f, .18f, .32f), 18, 1.0f, 0.5f);
			break;

		case GEO_SHOULDER_CANNON_BUTT:
			meshList[i] = MeshBuilder::GenerateHemisphere("GEO_SHOULDER_CANNON_BUTT", Color(.12f, .18f, .32f), 32, 36, 0.5f);
			break;

		case GEO_SHOULDER_SHOULDER:
			meshList[i] = MeshBuilder::GenerateCube("GEO_SHOULDER_SHOULDER", Color(.65f, .46f, .11f), 0.55f, 0.55f, .75f);
			break;

		case GEO_SHOULDER_ARMJOINT:
			meshList[i] = MeshBuilder::GenerateCylinder("GEO_SHOULDER_ARMJOINT", Color(.45f, .56f, .58f), 15, 1.5f, 0.25f);
			break;

		case GEO_SHOULDER_CHARGED_PARTICLE:
			meshList[i] = MeshBuilder::GenerateTorus("GEO_SHOULDER_CHARGED_PARTICLE", Color(0.8f, 0, 0), 36, 9, 2.0f, 0.1f);
			break;

		}
		meshList[i]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[i]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
		meshList[i]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
		meshList[i]->material.kShininess = 1.0f;

	}
}

void CharacterRenderer::renderMtbLeftShoulder(AnimMetabee& animMetabee) {

	// Shoulder
	modelStack->PushMatrix();


	RenderMesh(meshList[GEO_SHOULDER_SHOULDER], true);

	// Cannon
	modelStack->PushMatrix();
	modelStack->Translate(0, 1.05f, -1.3f);
	modelStack->Rotate(135, 1, 0, 0);
	RenderMesh(meshList[GEO_SHOULDER_CANNON_BARREL], true);

	// Cannon butt
	modelStack->PushMatrix();
	modelStack->Translate(0, 0.5f, 0);
	RenderMesh(meshList[GEO_SHOULDER_CANNON_BUTT], true);
	modelStack->PopMatrix();

	if (animMetabee.isChargedParticleOut()) {
		modelStack->PushMatrix();
		modelStack->Translate(0, -4, 0);
		animMetabee.popChargedParticle();
		RenderMesh(meshList[GEO_SHOULDER_CHARGED_PARTICLE], true);
		modelStack->PopMatrix();
	}
	modelStack->PopMatrix();


	// Joint
	modelStack->PushMatrix();
	modelStack->Translate(0, -0.5f, 0);
	RenderMesh(meshList[GEO_SHOULDER_ARMJOINT], true);
	modelStack->PopMatrix();

	modelStack->PopMatrix();

}

void CharacterRenderer::prepareMtbLeftArm() {
	for (unsigned i = GEO_ARM_AMMO_TOP; i <= GEO_ARM_HAND; i++) {

		switch (i) {

		case GEO_ARM_AMMO_TOP:
			meshList[i] = MeshBuilder::GenerateCube("GEO_ARM_AMMO_TOP", Color(.65f, .46f, .11f), .2f, .75f, .45f);
			break;

		case GEO_ARM_AMMO_BOTTOM:
			meshList[i] = MeshBuilder::GenerateCube("GEO_ARM_AMMO_BOTTOM", Color(.45f, .56f, .58f), .2f, .75f, .45f);
			break;

		case GEO_ARM_JOINT:
			meshList[i] = MeshBuilder::GenerateSphere("GEO_ARM_JOINT", Color(.45f, .56f, .58f), 16, 16, 0.3f);
			break;

		case GEO_ARM_MUZZLE:
			meshList[i] = MeshBuilder::GenerateHollowCylinder("GEO_ARM_MUZZLE", Color(.12f, .18f, .32f), 18, 1.0f, 0.15f, 0.10f);
			break;

		case GEO_ARM_HAND:
			meshList[i] = MeshBuilder::GenerateSphere("GEO_ARM_HAND", Color(.45f, .56f, .58f), 16, 16, 0.3f);
			break;


		}
		meshList[i]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[i]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
		meshList[i]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
		meshList[i]->material.kShininess = 1.0f;

	}
}

void CharacterRenderer::renderMtbLeftArm() {

	modelStack->PushMatrix();

	modelStack->PushMatrix();
	modelStack->Translate(0, 0, 0);
	RenderMesh(meshList[GEO_ARM_JOINT], true);
	modelStack->PopMatrix();

	modelStack->PushMatrix();
	modelStack->Translate(-0.2f, -1, 0);
	RenderMesh(meshList[GEO_ARM_AMMO_TOP], true);
	modelStack->PopMatrix();

	modelStack->PushMatrix();
	modelStack->Translate(0.2f, -1, 0);
	RenderMesh(meshList[GEO_ARM_AMMO_BOTTOM], true);
	modelStack->PopMatrix();

	modelStack->PushMatrix();
	modelStack->Translate(-0.2f, -2, -0.2f);
	RenderMesh(meshList[GEO_ARM_MUZZLE], true);
	modelStack->PopMatrix();

	modelStack->PushMatrix();
	modelStack->Translate(-0.2f, -2, 0.2f);
	RenderMesh(meshList[GEO_ARM_MUZZLE], true);
	modelStack->PopMatrix();

	modelStack->PushMatrix();
	modelStack->Translate(0.3f, -1.8f, 0);
	RenderMesh(meshList[GEO_ARM_HAND], true);
	modelStack->PopMatrix();

	modelStack->PopMatrix();

}

void CharacterRenderer::prepareMtbChest() {
	for (unsigned i = GEO_CHEST_NECK_JOINT; i <= GEO_CHEST_CHARGED_BALL; i++) {

		switch (i) {

		case GEO_CHEST_NECK_JOINT:
			meshList[i] = MeshBuilder::GenerateCylinder("GEO_CHEST_NECK_JOINT", Color(0, 0, 0), 15, 0.75f, 0.35f);
			break;

		case GEO_CHEST_LEFT_JOINT:
			meshList[i] = MeshBuilder::GenerateCylinder("GEO_CHEST_LEFT_JOINT", Color(0, 0, 0), 15, 0.75f, 0.2f);
			break;

		case GEO_CHEST_RIGHT_JOINT:
			meshList[i] = MeshBuilder::GenerateCylinder("GEO_CHEST_RIGHT_JOINT", Color(0, 0, 0), 15, 0.75f, 0.2f);
			break;

		case GEO_CHEST_ABDOMEN_JOINT:
			meshList[i] = MeshBuilder::GenerateCylinder("GEO_CHEST_ABDOMEN_JOINT", Color(0, 0, 0), 15, 0.75f, 0.35f);
			break;

		case GEO_CHEST_INNER_ARMOR_FRONT:
			meshList[i] = MeshBuilder::GenerateTriangleCuboid("GEO_CHEST_INNER_ARMOR_FRONT", Color(.65f, .46f, .11f), 1, -0.5f, 1);
			break;

		case GEO_CHEST_INNER_ARMOR_BACK:
			meshList[i] = MeshBuilder::GenerateCube("GEO_CHEST_INNER_ARMOR_BACK", Color(.65f, .46f, .11f), 1, 1, .75f);
			break;

		case GEO_CHEST_OUTER_ARMOR_FRONT:
			meshList[i] = MeshBuilder::GenerateTriangleCuboid("GEO_CHEST_OUTER_ARMOR_FRONT", Color(.45f, .56f, .58f), 1, -0.3f, 0.65f);
			break;
		case GEO_CHEST_CHARGED_BALL:
			meshList[i] = MeshBuilder::GenerateSphere("GEO_CHEST_CHARGED_BALL", Color(1, 1, .32f), 16, 16, 0.25f);
			break;


		}
		meshList[i]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[i]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
		meshList[i]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
		meshList[i]->material.kShininess = 1.0f;

	}

}

void CharacterRenderer::renderMtbChest(AnimMetabee& animMetabee) {

	// Neck
	modelStack->PushMatrix();
	modelStack->Translate(0, 1, 0);
	RenderMesh(meshList[GEO_CHEST_NECK_JOINT], true);
	modelStack->PopMatrix();

	// Left Joint
	modelStack->PushMatrix();
	modelStack->Translate(-1, 0.5f, 0);
	modelStack->Rotate(90, 0, 0, 1);
	RenderMesh(meshList[GEO_CHEST_LEFT_JOINT], true);
	modelStack->PopMatrix();

	// Right Joint
	modelStack->PushMatrix();
	modelStack->Translate(1, 0.5f, 0);
	modelStack->Rotate(90, 0, 0, 1);
	RenderMesh(meshList[GEO_CHEST_RIGHT_JOINT], true);
	modelStack->PopMatrix();

	// Abdomen Joint
	modelStack->PushMatrix();
	modelStack->Translate(0, -1, 0);
	RenderMesh(meshList[GEO_CHEST_ABDOMEN_JOINT], true);
	modelStack->PopMatrix();

	// Inner armor
	modelStack->PushMatrix();

	modelStack->Rotate(-90, 0, 0, 1);

	modelStack->PushMatrix();
	RenderMesh(meshList[GEO_CHEST_INNER_ARMOR_BACK], true);
	modelStack->PopMatrix();

	modelStack->Translate(0, 0, 1.75f);
	modelStack->Rotate(90, 1, 0, 0);
	RenderMesh(meshList[GEO_CHEST_INNER_ARMOR_FRONT], true);

	modelStack->PopMatrix();

	// Outer armor
	modelStack->PushMatrix();

	modelStack->Rotate(-90, 0, 0, 1);

	modelStack->Translate(0, 0, 1.75f);
	modelStack->Rotate(90, 1, 0, 0);
	RenderMesh(meshList[GEO_CHEST_OUTER_ARMOR_FRONT], true);

	modelStack->PopMatrix();

	// Energy Ball
	if (animMetabee.isChargedParticleOut()) {
		modelStack->PushMatrix();
		modelStack->Translate(0, 0.75f, 6);
		animMetabee.popEnergyBall();
		RenderMesh(meshList[GEO_CHEST_CHARGED_BALL], true);
		modelStack->PopMatrix();
	}


}

void CharacterRenderer::prepareMtbAbdomen() {
	for (unsigned i = GEO_ABDOMEN_INNER_ARMOR_TOP; i <= GEO_ABDOMEN_OUTER_ARMOR_BOTTOM; i++) {

		switch (i) {

		case GEO_ABDOMEN_INNER_ARMOR_TOP:
			meshList[i] = MeshBuilder::GenerateCube("GEO_ABDOMEN_INNER_ARMOR_TOP", Color(.65f, .46f, .11f), 1, .75f, .5f);
			break;

		case GEO_ABDOMEN_INNER_ARMOR_BOTTOM:
			meshList[i] = MeshBuilder::GenerateTriangleCuboid("GEO_ABDOMEN_INNER_ARMOR_BOTTOM", Color(.65f, .46f, .11f), 1, -0.5f, .75f);
			break;

		case GEO_ABDOMEN_OUTER_ARMOR_TOP:
			meshList[i] = MeshBuilder::GenerateCube("GEO_ABDOMEN_OUTER_ARMOR_TOP", Color(.45f, .56f, .58f), 0.65f, .85f, .51f);
			break;

		case GEO_ABDOMEN_OUTER_ARMOR_BOTTOM:
			meshList[i] = MeshBuilder::GenerateTriangleCuboid("GEO_ABDOMEN_OUTER_ARMOR_BOTTOM", Color(.45f, .56f, .58f), 0.65f, -0.5f, .85f);
			break;


		}
		meshList[i]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[i]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
		meshList[i]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
		meshList[i]->material.kShininess = 1.0f;

	}
}

void CharacterRenderer::renderMtbAbdomen() {

	// Inner Armor
	modelStack->PushMatrix();

	modelStack->Rotate(90, 1, 0, 0);

	modelStack->PushMatrix();
	RenderMesh(meshList[GEO_ABDOMEN_INNER_ARMOR_TOP], true);
	modelStack->PopMatrix();

	modelStack->Translate(0, 0, 1.5f);
	modelStack->Rotate(90, 1, 0, 0);
	RenderMesh(meshList[GEO_ABDOMEN_INNER_ARMOR_BOTTOM], true);

	modelStack->PopMatrix();

	// Outer armor
	modelStack->PushMatrix();

	modelStack->Rotate(90, 1, 0, 0);

	modelStack->PushMatrix();
	RenderMesh(meshList[GEO_ABDOMEN_OUTER_ARMOR_TOP], true);
	modelStack->PopMatrix();

	modelStack->Translate(0, 0, 1.5f);
	modelStack->Rotate(90, 1, 0, 0);
	RenderMesh(meshList[GEO_ABDOMEN_OUTER_ARMOR_BOTTOM], true);

	modelStack->PopMatrix();
}

void CharacterRenderer::prepareMtbLeftThigh() {
	meshList[GEO_THIGH_THIGH] = MeshBuilder::GenerateCylinder("GEO_THIGH_THIGH", Color(.45f, .56f, .58f), 15, 1.0f, 0.25f);
	meshList[GEO_THIGH_THIGH]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_THIGH_THIGH]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_THIGH_THIGH]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_THIGH_THIGH]->material.kShininess = 1.0f;
}

void CharacterRenderer::renderMtbLeftThigh() {
	modelStack->PushMatrix();
	modelStack->Translate(0, -0.5f, 0);
	RenderMesh(meshList[GEO_THIGH_THIGH], true);
	modelStack->PopMatrix();
}

void CharacterRenderer::prepareMtbLeftFoot() {

	for (unsigned i = GEO_FOOT_JOINT; i <= GEO_FOOT_FOOT; i++) {

		switch (i) {

		case GEO_FOOT_JOINT:
			meshList[i] = MeshBuilder::GenerateSphere("GEO_FOOT_JOINT", Color(.45f, .56f, .58f), 16, 16, 0.3f);
			break;

		case GEO_FOOT_SHIN:
			meshList[i] = MeshBuilder::GenerateCube("GEO_FOOT_ARMOR_BACK", Color(.65f, .46f, .11f), .4f, 1.0f, .5f);
			break;

		case GEO_FOOT_FOOT:
			meshList[i] = MeshBuilder::GenerateTriangleCuboid("GEO_FOOT_FOOT", Color(.45f, .56f, .58f), 0.4f, -0.5f, .35f);
			break;


		}
		meshList[i]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[i]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
		meshList[i]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
		meshList[i]->material.kShininess = 1.0f;

	}

}

void CharacterRenderer::renderMtbLeftFoot() {

	modelStack->PushMatrix();
	modelStack->Translate(0, 0, 0);
	RenderMesh(meshList[GEO_FOOT_JOINT], true);

	modelStack->PushMatrix();
	modelStack->Translate(0, -1.25f, 0);
	RenderMesh(meshList[GEO_FOOT_SHIN], true);
	modelStack->PopMatrix();

	modelStack->PushMatrix();
	modelStack->Translate(0, -1.9f, 1.5f);
	modelStack->Rotate(90, 1, 0, 0);
	RenderMesh(meshList[GEO_FOOT_FOOT], true);
	modelStack->PopMatrix();

	modelStack->PopMatrix();

}

void CharacterRenderer::prepareMtbSword() {
	meshList[GEO_SWORD_BLADE] = MeshBuilder::GenerateTrianglePyramid("GEO_SWORD_BLADE", Color(.12f, .18f, .32f), 0.20f, 7.5f);
	meshList[GEO_SWORD_BLADE]->material.kAmbient.Set(0.25f, 0.25f, 0.25f);
	meshList[GEO_SWORD_BLADE]->material.kDiffuse.Set(0.75f, 0.75f, 0.75f);
	meshList[GEO_SWORD_BLADE]->material.kSpecular.Set(0.25f, 0.25f, 0.25f);
	meshList[GEO_SWORD_BLADE]->material.kShininess = 1.0f;
}

void CharacterRenderer::renderMtbSword() {

	modelStack->PushMatrix();
	modelStack->Translate(0, 0.45f, 0);
	modelStack->Rotate(90, 0, 1, 0);
	RenderMesh(meshList[GEO_SWORD_BLADE], true);

	modelStack->PushMatrix();
	modelStack->Rotate(90, 0, 0, 1);
	modelStack->Translate(0, 0, 0);
	modelStack->Scale(0.5f, 0.5f, 0.5f);
	RenderMesh(meshList[GEO_ARM_AMMO_TOP], true);

	modelStack->PushMatrix();
	modelStack->Rotate(-90, 0, 0, 1);
	modelStack->Translate(0, -0.55f, 0);
	modelStack->Scale(0.5f, 0.5f, 0.5f);
	RenderMesh(meshList[GEO_ARM_AMMO_TOP], true);
	modelStack->PopMatrix();

	modelStack->PopMatrix();

	modelStack->PopMatrix();

}

void CharacterRenderer::renderMtbLeftBullet() {

	// Bullet 1
	modelStack->PushMatrix();
	modelStack->Translate(-0.2f, -2.0f, -0.2f);
	modelStack->Rotate(180, 0, 0, 1);
	modelStack->Scale(0.2f, 0.2f, 0.2f);
	RenderMesh(meshList[GEO_SHOULDER_CANNON_BARREL], true);


	modelStack->PushMatrix();
	modelStack->Translate(0, 0.5f, 0);
	RenderMesh(meshList[GEO_SHOULDER_CANNON_BUTT], true);
	modelStack->PopMatrix();

	modelStack->PopMatrix();

	// 2
	modelStack->PushMatrix();
	modelStack->Translate(-0.2f, -1.75f, 0.2f);
	modelStack->Rotate(180, 0, 0, 1);
	modelStack->Scale(0.2f, 0.2f, 0.2f);
	RenderMesh(meshList[GEO_SHOULDER_CANNON_BARREL], true);


	modelStack->PushMatrix();
	modelStack->Translate(0, 0.5f, 0);
	RenderMesh(meshList[GEO_SHOULDER_CANNON_BUTT], true);
	modelStack->PopMatrix();

	modelStack->PopMatrix();

	// 3
	modelStack->PushMatrix();
	modelStack->Translate(-0.2f, -1.5f, -0.2f);
	modelStack->Rotate(180, 0, 0, 1);
	modelStack->Scale(0.2f, 0.2f, 0.2f);
	RenderMesh(meshList[GEO_SHOULDER_CANNON_BARREL], true);


	modelStack->PushMatrix();
	modelStack->Translate(0, 0.5f, 0);
	RenderMesh(meshList[GEO_SHOULDER_CANNON_BUTT], true);
	modelStack->PopMatrix();

	modelStack->PopMatrix();

	// 4
	modelStack->PushMatrix();
	modelStack->Translate(-0.2f, -1.0f, -0.2f);
	modelStack->Rotate(180, 0, 0, 1);
	modelStack->Scale(0.2f, 0.2f, 0.2f);
	RenderMesh(meshList[GEO_SHOULDER_CANNON_BARREL], true);


	modelStack->PushMatrix();
	modelStack->Translate(0, 0.5f, 0);
	RenderMesh(meshList[GEO_SHOULDER_CANNON_BUTT], true);
	modelStack->PopMatrix();

	modelStack->PopMatrix();

	// 4
	modelStack->PushMatrix();
	modelStack->Translate(-0.2f, -1.0f, 0.2f);
	modelStack->Rotate(180, 0, 0, 1);
	modelStack->Scale(0.2f, 0.2f, 0.2f);
	RenderMesh(meshList[GEO_SHOULDER_CANNON_BARREL], true);


	modelStack->PushMatrix();
	modelStack->Translate(0, 0.5f, 0);
	RenderMesh(meshList[GEO_SHOULDER_CANNON_BUTT], true);
	modelStack->PopMatrix();

	modelStack->PopMatrix();


}