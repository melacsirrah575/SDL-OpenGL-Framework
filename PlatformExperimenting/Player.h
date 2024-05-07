#ifndef __PLAYER_H
#define __PLAYER_H
#include "AnimatedGLTexture.h"
#include "AudioManager.h"
#include "InputManager.h"
#include "PhysEntity.h"

using namespace SDLFramework;

class Player : public PhysEntity {
private:
	Timer * mTimer;
	InputManager * mInput;
	AudioManager * mAudio;

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

	Vector2 mXMoveBounds;
	Vector2 mYMoveBounds;

private:
	void HandleMovement();

public:
	Player();
	~Player();

	void Visible(bool visible);
	bool IsAnimating();

	int Score();
	int Lives();

	void AddScore(int change);

	// Inherited from PhysEntity
	void Hit(PhysEntity * other) override;
	
	bool WasHit();

	void Update() override;
	void Render() override;
};
#endif