#include "Enemy.h"
#include "PhysicsManager.h"

Player * Enemy::sPlayer = nullptr;

void Enemy::CurrentPlayer(Player * player) {
	sPlayer = player;
}

Enemy::Enemy() {
	mTimer = Timer::Instance();

	mCurrentState = Alive;

	//Texture Assignment needed!
	mTexture = nullptr;
	mDeathAnimation = nullptr;

	mSpeed = 400.0f;

	mId = PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::Hostile);
}

Enemy::~Enemy() {
	mTimer = nullptr;

	mTexture = nullptr;
	delete mTexture;
	mDeathAnimation = nullptr;
	delete mDeathAnimation;
}

void Enemy::HandleStates() {
	switch (mCurrentState) {
	case Alive:
		//Handle Alive Code 
		break;
	case Dead:
		//Handle Dead Code
		break;
	}
	//Add new states as needed!
}




void Enemy::RenderStates() {
	switch (mCurrentState) {
	case Alive:
		//Handle Alive Code 
		break;
	case Dead:
		//Handle Dead Code
		break;
	}
	//Add new states as needed!

	PhysEntity::Render();
}

bool Enemy::IgnoreCollisions()
{
	return mCurrentState == Dead;
}

void Enemy::Hit(PhysEntity * other) {

}

Enemy::States Enemy::CurrentState() {
	return mCurrentState;
}


bool Enemy::InDeathAnimation() {
	return mDeathAnimation->IsAnimating();
}

void Enemy::Update() {
	if (Active()) {
		HandleStates();
	}
}

void Enemy::Render() {
	if (Active()) {
		RenderStates();
	}
}
