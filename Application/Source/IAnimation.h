#ifndef I_ANIMATION_H
#define I_ANIMATION_H

#include "MatrixStack.h"

// Abstract Class, used for deriving animated characters
class IAnimation {

public:
	virtual void ProcessAnimation(float dt) = 0;
	virtual void ProcessMovement(float dt) = 0;
	

};

#endif