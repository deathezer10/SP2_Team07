#ifndef CHARACTER_RENDERER_H
#define CHARACTER_RENDERER_H

#include "Geometry.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "AnimMetabee.h"


/*
\brief Responsible for the creation & rendering of specific character parts
*/
class CharacterRenderer {


public:
	CharacterRenderer(Mesh * meshlist, unsigned * mParameter, MS* mStack, MS* vStack, MS* pStack);
	~CharacterRenderer();

	void renderMtbHead();

	void renderMtbLeftShoulder(AnimMetabee& animMetabee);
	void renderMtbLeftArm();

	void renderMtbChest(AnimMetabee& animMetabee);
	void renderMtbAbdomen();

	void renderMtbLeftThigh();
	void renderMtbLeftFoot();

	void renderMtbSword();
	
	void renderMtbLeftBullet();


private:
	MS *modelStack, *viewStack, *projectionStack;

	unsigned * m_parameters;

	Mesh* meshList[NUM_GEOMETRY];

	void RenderMesh(Mesh *mesh, bool enableLight);

	void prepareMtbHead();

	void prepareMtbLeftShoulder();
	void prepareMtbLeftArm();

	void prepareMtbChest();
	void prepareMtbAbdomen();

	void prepareMtbLeftThigh();
	void prepareMtbLeftFoot();

	void prepareMtbSword();


};




#endif