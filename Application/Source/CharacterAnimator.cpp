#include "CharacterAnimator.h"




CharacterAnimator::CharacterAnimator(MS* mStack) : mStack_(mStack), animMetabee_(mStack) {
}

CharacterAnimator::~CharacterAnimator() {
}

void CharacterAnimator::UpdateAnimation(float dt) {

	animMetabee_.ProcessMovement(dt);
	animMetabee_.ProcessAnimation(dt);

}