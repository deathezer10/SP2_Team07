#ifndef BULLET_H
#define BULLET_H


#include "Object.h"

// Bullet Object, Collides with Rocks & Slimes
class Bullet : public Object {


public:
	Bullet(Scene* scene, Vector3 pos, int damage);
	Bullet(Scene* scene, Vector3 pos, int damage, Vector3 rotation, Vector3 direction);
	~Bullet() {};

	virtual bool update();
	virtual void collisionHit(Vector3& hitPos);

private:
	bool _isEnemyBullet = false;

	int _bulletDamage;
	float _bulletSpeed;

	const float _bulletMaxDistance = 500.0f;

	Vector3 _startingPosition;
	Vector3 _direction;

};
#endif