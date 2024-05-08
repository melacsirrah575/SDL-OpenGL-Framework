#ifndef __PHYSENTITY_H
#define __PHYSENTITY_H
#include <vector>
#include "Collider.h"

class PhysEntity : public GameEntity {
protected:
	unsigned long mId;

	bool mIsColliding = false;

	std::vector<Collider *> mColliders;

	Collider * mBroadPhaseCollider;

	void AddCollider(Collider * collider, Vector2 localPos = Vec2_Zero);

	virtual bool IgnoreCollisions();

	std::string mName;

public:
	PhysEntity();
	virtual ~PhysEntity();

	unsigned long GetId();

	bool CheckCollision(PhysEntity * other);

	virtual void Hit(PhysEntity * other) { }

	std::string GetName();

	void SetIsColliding(bool isColliding) { mIsColliding = isColliding; }

	bool GetIsColliding() { return mIsColliding; }

	virtual void Render() override;
};
#endif