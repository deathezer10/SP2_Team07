#ifndef CHARACTER_ANIMATOR_H
#define CHARACTER_ANIMATOR_H

#include "MatrixStack.h"
#include "AnimMetabee.h"

class Scene;

// Stores all the character animations
class CharacterAnimator {


public:
	CharacterAnimator(Scene* scene);
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