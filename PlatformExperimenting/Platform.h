#pragma once
#include "PhysEntity.h"

using namespace SDLFramework;

class Platform : public PhysEntity {
private:
	bool mCanBeStoodOn;
	bool mCanBeJumpedThrough;

	Texture* mPlatformTexture;

public:
	Platform(bool canBeStoodOn, bool canBeJumpedThrough, Vector2 Position, GLTexture* texture,
		Vector2 scale = Vector2(1.0f, 1.0f), Vector2 boxColliderSize = Vec2_Zero, Vector2 colliderOffset = Vec2_Zero);
	~Platform();

	// Inherited from PhysEntity
	void Hit(PhysEntity* other) override;

	bool GetCanBeStoodOn();
	bool GetCanBeJumpedThrough();

	Texture* GetTexture();

	void Update() override;
	void Render() override;
};