#include "Player.h"
#include "BoxCollider.h"
#include "PhysicsManager.h"
#include "Platform.h"
#include <sstream>

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

	if (mInput->KeyPressed(SDL_SCANCODE_SPACE) && mGrounded) {
		mPlayerJumped = true;
		mGrounded = false;
		mJumpTime = 0.0f;
	}

	if (mPlayerJumped) {
		if (mJumpTime < 0.5f * mJumpSpeed) {
			Translate(-Vec2_Up * mMoveSpeed * mTimer->DeltaTime(), World);
		}
		else {
			if (mGrounded) {
				mPlayerJumped = false;
				return;
			}
			Translate(Vec2_Up * mMoveSpeed * mTimer->DeltaTime(), World);
		}

		mJumpTime += mJumpSpeed * mTimer->DeltaTime();

		if (mJumpTime >= mJumpSpeed) {
			mPlayerJumped = false;
		}
	}

	if ((!mGrounded || !mIsColliding) && !mPlayerJumped) {
		Translate(Vec2_Up * mMoveSpeed * mTimer->DeltaTime(), World);
	}

	Vector2 pos = Position(Local);
	if (pos.x <= mXMoveBounds.x) {
		pos.x = mXMoveBounds.x;
		Graphics::Instance()->SetCameraPosition(
			Graphics::Instance()->GetCameraX() - mMoveSpeed * mTimer->DeltaTime(),
			Graphics::Instance()->GetCameraY());
	}
	else if (pos.x >= mXMoveBounds.y) {
		pos.x = mXMoveBounds.y;
	}
	if (pos.y <= mYMoveBounds.x) {
		pos.y = mYMoveBounds.x;
	}
	else if (pos.y >= mYMoveBounds.y) {
		pos.y = mYMoveBounds.y;
		mGrounded = true;
	}

	Position(pos);
	//std::cout << "Player Position: " << Position().x << ", " << Position().y << std::endl;
	//Graphics::Instance()->SetCameraPosition(Position().x, Position().y);
}


Player::Player() {
	mTimer = Timer::Instance();
	mInput = InputManager::Instance();
	mAudio = AudioManager::Instance();
	mPlatforms = PlatformManager::Instance();

	mVisible = false;
	mAnimating = false;
	mWasHit = false;
	mPlayerJumped = false;
	mGrounded = true;

	mScore = 0;
	mLives = 2;
	
	mPlayerTexture = new GLTexture("Black.png");
	mPlayerTexture->Parent(this);
	mPlayerTexture->Position(Vec2_Zero);
	mPlayerTexture->Scale(Vector2(0.2f, 0.6f));

	mMoveSpeed = 300.0f;
	mJumpSpeed = 300.0f;
	mJumpHeight = 5.0f;
	mJumpSpeed = 10.0f;
	mJumpTime = 0.0f;

	mXMoveBounds = Vector2(0.0f, Graphics::SCREEN_WIDTH);
	mYMoveBounds = Vector2(100.0f, Graphics::SCREEN_HEIGHT);

	mDeathAnimation = nullptr;

	AddCollider(new BoxCollider(Vector2(mPlayerTexture->ScaledDimensions().x, mPlayerTexture->ScaledDimensions().y)));

	mId = PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::Friendly);
	mName = "Player";
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

void Player::Hit(PhysEntity * other) {
	if (other->GetName() == mPlatforms->GetPlatform(other->GetId())->GetName()) {
		if (mPlatforms->GetPlatform(other->GetId())->GetCanBeStoodOn() && Position().y + mPlayerTexture->ScaledDimensions().y >= mPlatforms->GetPlatformPosition(other->GetId()).y) {
			if (Position().x >= mPlatforms->GetPlatformPosition(other->GetId()).x - mPlatforms->GetPlatform(other->GetId())->GetTexture()->ScaledDimensions().x / 2 &&
				Position().x <= mPlatforms->GetPlatformPosition(other->GetId()).x + mPlatforms->GetPlatform(other->GetId())->GetTexture()->ScaledDimensions().x / 2) {
 				mGrounded = true;
			}
		}
		else {
			mGrounded = false;
		}
	}
}

bool Player::WasHit() {
	return mWasHit;
}

void Player::Update() {

	HandleMovement();
}

void Player::Render() {
	mPlayerTexture->Render();


	PhysEntity::Render();
}
