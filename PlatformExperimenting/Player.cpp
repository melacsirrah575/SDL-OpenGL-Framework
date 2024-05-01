#include "Player.h"
#include "BoxCollider.h"
#include "PhysicsManager.h"

void Player::HandleMovement() {
	if (mInput->KeyDown(SDL_SCANCODE_RIGHT) || mInput->KeyDown(SDL_SCANCODE_D)) {
		Translate(Vec2_Right * mMoveSpeed * mTimer->DeltaTime(), World);
	}
	else if (mInput->KeyDown(SDL_SCANCODE_LEFT) || mInput->KeyDown(SDL_SCANCODE_A)) {
		Translate(-Vec2_Right * mMoveSpeed * mTimer->DeltaTime(), World);
	}

	//if (mInput->KeyDown(SDL_SCANCODE_UP) || mInput->KeyDown(SDL_SCANCODE_W)) {
	//	Translate(-Vec2_Up * mMoveSpeed * mTimer->DeltaTime(), World);
	//}
	//else if (mInput->KeyDown(SDL_SCANCODE_DOWN) || mInput->KeyDown(SDL_SCANCODE_S)) {
	//	Translate(Vec2_Up * mMoveSpeed * mTimer->DeltaTime(), World);
	//}

	if (mInput->KeyPressed(SDL_SCANCODE_SPACE)) {
		mPlayerJumped = true;
		mJumpTime = 0.0f;
	}

	if (mPlayerJumped) {
		if (mJumpTime < 0.5f * mJumpSpeed) {
			Translate(-Vec2_Up * mMoveSpeed * mTimer->DeltaTime(), World);
		}
		else {
			Translate(Vec2_Up * mMoveSpeed * mTimer->DeltaTime(), World);
		}

		mJumpTime += mJumpSpeed * mTimer->DeltaTime();

		if (mJumpTime >= mJumpSpeed) {
			mPlayerJumped = false;
		}
	}

	Vector2 pos = Position(Local);
	if (pos.x < mXMoveBounds.x) {
		pos.x = mXMoveBounds.x;
	}
	else if (pos.x > mXMoveBounds.y) {
		pos.x = mXMoveBounds.y;
	}
	if (pos.y < mYMoveBounds.x) {
		pos.y = mYMoveBounds.x;
	}
	else if (pos.y > mYMoveBounds.y) {
		pos.y = mYMoveBounds.y;
	}

	Position(pos);
}


Player::Player() {
	mTimer = Timer::Instance();
	mInput = InputManager::Instance();
	mAudio = AudioManager::Instance();

	mVisible = false;
	mAnimating = false;
	mWasHit = false;
	mPlayerJumped = false;

	mScore = 0;
	mLives = 2;
	
	mPlayerTexture = nullptr;

	mMoveSpeed = 300.0f;
	mJumpSpeed = 300.0f;
	mJumpHeight = 5.0f;
	mJumpSpeed = 10.0f;
	mJumpTime = 0.0f;

	mXMoveBounds = Vector2(0.0f, Graphics::SCREEN_WIDTH);
	mYMoveBounds = Vector2(100.0f, Graphics::SCREEN_HEIGHT);

	mDeathAnimation = nullptr;

	AddCollider(new BoxCollider(Vector2(16.0f, 67.0f)));

	mId = PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::Friendly);
}

Player::~Player() {
	mTimer = nullptr;
	mInput = nullptr;
	mAudio = nullptr;

	delete mPlayerTexture;
	mPlayerTexture = nullptr;

	delete mDeathAnimation;
	mDeathAnimation = nullptr;
}

void Player::Visible(bool visible) {
	mVisible = visible;
}

bool Player::IsAnimating() {
	return mAnimating;
}

int Player::Score() {
	return mScore;
}

int Player::Lives() {
	return mLives;
}

void Player::AddScore(int change) {
	mScore += change;
}

bool Player::IgnoreCollisions()
{
	return !mVisible || mAnimating;
}

void Player::Hit(PhysEntity * other) {
	mLives -= 1;
	mWasHit = true;
}

bool Player::WasHit() {
	return mWasHit;
}

void Player::Update() {

	HandleMovement();
}

void Player::Render() {
	if (mVisible) {
		//Cannot currently render because texture is a nullptr!
		//mPlayerTexture->Render();
	}

	PhysEntity::Render();
}
