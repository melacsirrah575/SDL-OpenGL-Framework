#ifndef __PLAYER_H
#define __PLAYER_H
#include "AnimatedGLTexture.h"
#include "AudioManager.h"
#include "InputManager.h"
#include "PhysEntity.h"
#include "PlatformManager.h"

using namespace SDLFramework;

class Player : public PhysEntity {
private:
	Timer * mTimer;
	InputManager * mInput;
	AudioManager * mAudio;
	PlatformManager* mPlatforms;

	bool mVisible;
	bool mAnimating;
	bool mWasHit;
	bool mPlayerJumped;
	bool mGrounded;

	int mScore;
	int mLives;

	Texture * mPlayerTexture;
	AnimatedGLTexture * mDeathAnimation;

	float mMoveSpeed;
	float mJumpHeight;
	float mJumpSpeed;
	float mJumpTime;
	float mXOffset;
	float mYOffset;

	Vector2 mXScrollBoundryOffset;
	Vector2 mYScrollBoundryOffset;

private:
	void HandleMovement();
	void HandleJumping();
	void HandleBoundsCheckingAndCameraScrolling();

public:
	Player();
	~Player();

	void Visible(bool visible);
	bool IsAnimating() const;

	int GetScore() const;
	int GetLives() const;

	void AddScore(int change);

	// Inherited from PhysEntity
	void Hit(PhysEntity * other) override;
	
	bool GetWasHit() const;

	void Update() override;
	void Render() override;
};
#endif