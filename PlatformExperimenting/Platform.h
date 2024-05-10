#pragma once
#include "PhysEntity.h"

using namespace SDLFramework;

class Platform : public PhysEntity {
private:
	bool mCanBeStoodOn;

	Texture* mPlatformTexture;

public:
	Platform(bool canBeStoodOn, Vector2 Position, GLTexture* texture,
		Vector2 scale = Vector2(1.0f, 1.0f), Vector2 colliderOffset = Vec2_Zero);
	~Platform();

	// Inherited from PhysEntity
	void Hit(PhysEntity* other) override;

	bool GetCanBeStoodOn();

	Texture* GetTexture();

	void Update() override;
	void Render() override;
};