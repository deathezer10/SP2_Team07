#ifndef BULLET_H
#define BULLET_H


#include "Object.h"

// Bullet Object, Collides with Rocks & Slimes
class Bullet : public Object {


public:
	Bullet(Scene* scene, Vector3 pos, int damage, bool isEnemy = false);
	~Bullet() {};

	virtual bool checkInteract();
	virtual void collisionHit(Vector3& hitPos);
	virtual void render();

private:
	bool _isEnemyBullet;

	int _bulletDamage;
	float _bulletSpeed;

	const float _interactDistance = 5.f;
	const float _bulletMaxDistance = 300.0f;

	Vector3 _startingPosition;
	Vector3 _direction;

};
#endif