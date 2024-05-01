#pragma once
#include "PhysEntity.h"

using namespace SDLFramework;

class Platform : public PhysEntity {
private:
	bool mWasHit;
	bool mCanBeStoodOn;

	Texture* mPlatformTexture;

public:
	Platform(bool canBeStoodOn);
	~Platform();

	// Inherited from PhysEntity
	bool IgnoreCollisions() override;
	void Hit(PhysEntity* other) override;

	bool WasHit();
	bool GetCanBeStoodOn();

	void Update() override;
	void Render() override;
};