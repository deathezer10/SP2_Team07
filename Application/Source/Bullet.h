#ifndef BULLET_H
#define BULLET_H


#include "Object.h"

// Bullet Object, Collides with Rocks & Slimes
class Bullet : public Object {


public:
	Bullet(Scene* scene, Vector3 pos);
    ~Bullet() {};
		
	virtual void checkInteract();
    virtual void interact();
    virtual void render();

private:
    const float _interactDistance = 1.f;
    const float _bulletSpeed = 20.0f;
	const float _bulletMaxDistance = 50.0f;
	
	Vector3 _defaultPosition;
	float _defaultYaw;
	float _defaultPitch;
	
};
#endif