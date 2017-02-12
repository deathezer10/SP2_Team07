#ifndef CHARACTER_ANIMATOR_H
#define CHARACTER_ANIMATOR_H

#include "MatrixStack.h"
#include "AnimMetabee.h"

// Stores all the character animations
class CharacterAnimator {


public:
	CharacterAnimator(MS* mStack);
	~CharacterAnimator();
	void UpdateAnimation(float dt);

	AnimMetabee &getAnimMetabee() {
		return animMetabee_;
	}

private:
	MS* mStack_;
	AnimMetabee animMetabee_;


};

#endif