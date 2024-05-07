#pragma once
#include "PhysEntity.h"

using namespace SDLFramework;

class Platform : public PhysEntity {
private:
	bool mCanBeStoodOn;

	Texture* mPlatformTexture;

public:
	Platform(bool canBeStoodOn, Vector2 boxCollider, Vector2 Position, Vector2 colliderOffset = Vec2_Zero);
	~Platform();

	// Inherited from PhysEntity
	void Hit(PhysEntity* other) override;

	bool GetCanBeStoodOn();

	void Update() override;
	void Render() override;
};