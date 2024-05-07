#include "Player.h"
#include "BoxCollider.h"
#include "PhysicsManager.h"
#include "Platform.h"

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
			//std::cout << "mGrounded: " << mGrounded << std::endl;
			Translate(-Vec2_Up * mMoveSpeed * mTimer->DeltaTime(), World);
		}
		else {
			if (mGrounded) return;
			Translate(Vec2_Up * mMoveSpeed * mTimer->DeltaTime(), World);
		}

		mJumpTime += mJumpSpeed * mTimer->DeltaTime();

		if (mJumpTime >= mJumpSpeed) {
			mPlayerJumped = false;
			mGrounded = true;
		}
	}

	//std::cout << "mGrounded: " << mGrounded << std::endl;

	if (!mGrounded && !mPlayerJumped) {
		Translate(Vec2_Up * mMoveSpeed * mTimer->DeltaTime(), World);
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
		mGrounded = true;
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
	mGrounded = true;

	mScore = 0;
	mLives = 2;
	
	mPlayerTexture = nullptr;

	mMoveSpeed = 300.0f;
	mJumpSpeed = 300.0f;
	mJumpHeight = 5.0f;
	mJumpSpeed = 10.0f;
	mJumpTime = 0.0f;

	mXMoveBounds = Vector2(0.0f, Graphics::SCREEN_WIDTH);
	//PAIRING BOTTOM Y BOUNDS WITH PLAYER POSITION IN PLAYSCREEN!!
	mYMoveBounds = Vector2(100.0f, Graphics::SCREEN_HEIGHT * 0.8f);

	mDeathAnimation = nullptr;

	AddCollider(new BoxCollider(Vector2(16.0f, 67.0f)));

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
	//mLives -= 1;
	std::cout << "Hit!" << std::endl;
	if (other->GetName() == "Platform") {
		std::cout << "Platform hit!" << std::endl;
		//If Platform is Standable && Player's Y Pos is greater than Platform's Y Pos
		Platform testPlatform = other;
		//USING 67.0f / 2 BECAUSE I DON'T HAVE A TEXTURE AND THE COLLIDER IS 67
		if (testPlatform.GetCanBeStoodOn() && Position().y + (67.0f / 2) >= other->Position().y) {
			//USING 75 BECAUSE I DON'T HAVE A TEXTURE AND THE COLLIDER IS 150
			if (Position().x >= other->Position().x - 75 &&
				Position().x <= other->Position().x + 75) {
				mGrounded = true;
				//std::cout << "mGrounded: " << mGrounded << std::endl;
			}
		} 
		else {
			mGrounded = false;
			//std::cout << "mGrounded: " << mGrounded << std::endl;
		}
		//else
			//Player's Y moveBounds go back to default
	}
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
