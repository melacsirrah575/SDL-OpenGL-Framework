#include "Platform.h"
#include "BoxCollider.h"
#include "PhysicsManager.h"

Platform::Platform(bool canBeStoodOn, Vector2 boxCollider, Vector2 position, Vector2 scale, Vector2 colliderOffset) :
	mCanBeStoodOn(canBeStoodOn) {

	//TODO: Assign this with a Texture if we want an image!
	mPlatformTexture = new GLTexture("Black.png");
	if (mPlatformTexture != nullptr) {
		mPlatformTexture->Parent(this);
		mPlatformTexture->Position(Vec2_Zero);
		mPlatformTexture->Scale(scale);
	}

	Position(position);
	AddCollider(new BoxCollider(boxCollider), colliderOffset);

	mId = PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::Platforms);
	mName = "Platform";
	mName += std::to_string(mId);
}

Platform::~Platform() {
	delete mPlatformTexture;
	mPlatformTexture = nullptr;
}

bool Platform::GetCanBeStoodOn() {
	return mCanBeStoodOn;
}

Texture* Platform::GetTexture() {
	return mPlatformTexture;
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