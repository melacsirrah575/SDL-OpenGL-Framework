#include "Platform.h"
#include "BoxCollider.h"
#include "PhysicsManager.h"

Platform::Platform(bool canBeStoodOn) : 
	mCanBeStoodOn(canBeStoodOn) {

	//TODO: Assign this with a Texture if we want an image!
	mPlatformTexture = nullptr;
	if (mPlatformTexture != nullptr) {
		mPlatformTexture->Parent(this);
		mPlatformTexture->Position(Vec2_Zero);
	}

	AddCollider(new BoxCollider(Vector2(150.0f, 16.0f)));

	mId = PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::Platforms);
	mName = "Platform";
}

Platform::~Platform() {
	delete mPlatformTexture;
	mPlatformTexture = nullptr;
}

bool Platform::GetCanBeStoodOn() {
	return mCanBeStoodOn;
}

void Platform::Hit(PhysEntity* other) {

}

void Platform::Update() {

}

void Platform::Render() {
	if (mPlatformTexture != nullptr) {
		mPlatformTexture->Render();
	}

	PhysEntity::Render();
}