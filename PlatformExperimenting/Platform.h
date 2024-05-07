#pragma once
#include "PhysEntity.h"

using namespace SDLFramework;

class Platform : public PhysEntity {
private:
	bool mCanBeStoodOn;

	Texture* mPlatformTexture;

public:
	Platform(bool canBeStoodOn);
	~Platform();

	// Inherited from PhysEntity
	void Hit(PhysEntity* other) override;

	bool GetCanBeStoodOn();

	void Update() override;
	void Render() override;
};