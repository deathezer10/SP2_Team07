#include "CharacterAnimator.h"


#include "Scene.h"

CharacterAnimator::CharacterAnimator(Scene* scene) : mStack_(&scene->modelStack), animMetabee_(&scene->modelStack) {
}

CharacterAnimator::~CharacterAnimator() {
}

void CharacterAnimator::UpdateAnimation(float dt) {

	animMetabee_.ProcessMovement(dt);
	animMetabee_.ProcessAnimation(dt);

}