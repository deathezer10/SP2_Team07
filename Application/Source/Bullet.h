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
    float _bulletSpeed = 50.f;
	const float _bulletMaxDistance = 100.0f;
	
	Vector3 _startingPosition;
	Vector3 _direction;

};
#endif