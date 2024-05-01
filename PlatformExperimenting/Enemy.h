#ifndef __ENEMY_H
#define __ENEMY_H
#include "AnimatedGLTexture.h"
#include "PhysEntity.h"
#include "Player.h"

class Enemy : public PhysEntity {
public:
	enum States { Alive, Dead };

protected:
	static Player * sPlayer;

	Timer * mTimer;

	Texture * mTexture;
	AnimatedGLTexture* mDeathAnimation;

	States mCurrentState;

	float mSpeed;


protected:
	void HandleStates();

	void RenderStates();

	// Inherited from PhysEntity
	bool IgnoreCollisions() override;

public:
	static void CurrentPlayer(Player * player);

	Enemy();
	virtual ~Enemy();

	// Inherited from PhysEntity
	virtual void Hit(PhysEntity * other) override;
	
	States CurrentState();

	bool InDeathAnimation();

	void Update() override;
	void Render() override;
};
#endif