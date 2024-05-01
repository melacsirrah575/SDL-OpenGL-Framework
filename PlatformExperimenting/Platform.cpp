#include "Platform.h"
#include "BoxCollider.h"
#include "PhysicsManager.h"

Platform::Platform(bool canBeStoodOn) : 
	mCanBeStoodOn(canBeStoodOn) {

	//TODO: Assign this with a Texture if we want an image!
	mPlatformTexture = nullptr;

	AddCollider(new BoxCollider(Vector2(16.0f, 150.0f)));

	mId = PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::Platforms);
}

Platform::~Platform() {
	delete mPlatformTexture;
	mPlatformTexture = nullptr;
}