#include "Player.h"
#include "BoxCollider.h"
#include "PhysicsManager.h"
#include "Platform.h"
#include <sstream>
#include "ScreenManager.h"

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
	if (pos.x <= mXScrollBoundryOffset.x) {
		Graphics::Instance()->SetCameraPosition(
			Graphics::Instance()->GetCameraX() - mMoveSpeed * mTimer->DeltaTime(),
			Graphics::Instance()->GetCameraY());

		mXScrollBoundryOffset.x -= mMoveSpeed * mTimer->DeltaTime();
		mXScrollBoundryOffset.y -= mMoveSpeed * mTimer->DeltaTime();

	}
	else if (pos.x >= mXScrollBoundryOffset.y) {
		Graphics::Instance()->SetCameraPosition(
			Graphics::Instance()->GetCameraX() + mMoveSpeed * mTimer->DeltaTime(),
			Graphics::Instance()->GetCameraY());

		mXScrollBoundryOffset.x += mMoveSpeed * mTimer->DeltaTime();
		mXScrollBoundryOffset.y += mMoveSpeed * mTimer->DeltaTime();
	}
	if (pos.y <= mYScrollBoundryOffset.x) {
		Graphics::Instance()->SetCameraPosition(
			Graphics::Instance()->GetCameraX(),
			Graphics::Instance()->GetCameraY() - mMoveSpeed * mTimer->DeltaTime());

		mYScrollBoundryOffset.x -= mMoveSpeed * mTimer->DeltaTime();
		mYScrollBoundryOffset.y -= mMoveSpeed * mTimer->DeltaTime();

	}
	else if (pos.y >= mYScrollBoundryOffset.y) {

		if (mYScrollBoundryOffset.y + mYOffset < Graphics::SCREEN_HEIGHT) {
			Graphics::Instance()->SetCameraPosition(
				Graphics::Instance()->GetCameraX(),
				Graphics::Instance()->GetCameraY() + mMoveSpeed * mTimer->DeltaTime());

			mYScrollBoundryOffset.x += mMoveSpeed * mTimer->DeltaTime();
			mYScrollBoundryOffset.y += mMoveSpeed * mTimer->DeltaTime();
		}
		else {
			Graphics::Instance()->SetCameraPosition(
				Graphics::Instance()->GetCameraX(), 0);
			mYScrollBoundryOffset.y = Graphics::SCREEN_HEIGHT;
			if (pos.y >= Graphics::SCREEN_HEIGHT) {
				mGrounded = true;
				pos.y = Graphics::SCREEN_HEIGHT;
			}
		}
	}

	Position(pos);
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
	
	mPlayerTexture = new GLTexture("Black.png", 0, 0, 100, 100, {255, 255, 255});
	mPlayerTexture->Parent(this);
	mPlayerTexture->Position(Vec2_Zero);
	mPlayerTexture->Scale(Vector2(0.2f, 0.6f));

	mMoveSpeed = 300.0f;
	mJumpSpeed = 300.0f;
	mJumpHeight = 5.0f;
	mJumpSpeed = 10.0f;
	mJumpTime = 0.0f;
	mXOffset = 250.0f;
	mYOffset = 100.0f;

	mXScrollBoundryOffset = Vector2(mXOffset, Graphics::SCREEN_WIDTH - mXOffset);
	//Multiplying the Offset by 3 here because I want the scrollBoundry to be further down from the top of the screen.
	mYScrollBoundryOffset = Vector2(mYOffset * 3, Graphics::SCREEN_HEIGHT - mYOffset);

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
		if (mPlatforms->GetPlatform(other->GetId())->GetCanBeStoodOn() &&
			Position().y + (mPlayerTexture->ScaledDimensions().y * 0.5) <= mPlatforms->GetPlatformPosition(other->GetId()).y) {
			if (Position().x > mPlatforms->GetPlatformPosition(other->GetId()).x - mPlatforms->GetPlatform(other->GetId())->GetTexture()->ScaledDimensions().x / 2 &&
				Position().x < mPlatforms->GetPlatformPosition(other->GetId()).x + mPlatforms->GetPlatform(other->GetId())->GetTexture()->ScaledDimensions().x / 2) {
				mGrounded = true;
			}
		}
		else if (!mPlatforms->GetPlatform(other->GetId())->GetCanBeJumpedThrough() &&
			Position().y - (mPlayerTexture->ScaledDimensions().y * 0.5) >= mPlatforms->GetPlatformPosition(other->GetId()).y) {
			if (mJumpTime <= 0.5 * mJumpSpeed) {
				mJumpTime = 0.5 * mJumpSpeed;
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


	//PhysEntity::Render();
}
