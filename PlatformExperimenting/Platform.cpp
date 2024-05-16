#include "Platform.h"
#include "BoxCollider.h"
#include "PhysicsManager.h"


Platform::Platform(bool canBeStoodOn, bool canBeJumpedThrough, Vector2 position, GLTexture* texture, Vector2 scale, Vector2 boxColliderSize, Vector2 colliderOffset) :
	mCanBeStoodOn(canBeStoodOn), mCanBeJumpedThrough(canBeJumpedThrough) {

	mPlatformTexture = texture;
	if (mPlatformTexture != nullptr) {
		mPlatformTexture->Parent(this);
		mPlatformTexture->Position(Vec2_Zero);
		mPlatformTexture->Scale(scale);
	}

	Position(position);

	if (boxColliderSize.x == 0.0f && boxColliderSize.y == 0.0f) {
		AddCollider(new BoxCollider(Vector2(mPlatformTexture->ScaledDimensions().x, mPlatformTexture->ScaledDimensions().y)), colliderOffset);
	}
	else {
		AddCollider(new BoxCollider(Vector2(boxColliderSize.x, boxColliderSize.y)), colliderOffset);
	}

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

bool Platform::GetCanBeJumpedThrough() {
	return mCanBeJumpedThrough;
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