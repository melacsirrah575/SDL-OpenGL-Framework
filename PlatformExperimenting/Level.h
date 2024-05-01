#ifndef __LEVEL_H
#define __LEVEL_H
#include "Player.h"
#include "tinyxml2.h"

using namespace tinyxml2;

class Level : public GameEntity {
public:
	enum LevelStates { Running, Finished, GameOver };

private:
	Timer * mTimer;

	int mStage;
	bool mStageStarted;

	Player * mPlayer;
	bool mPlayerHit;
	float mRespawnDelay;
	float mRespawnTimer;
	float mRespawnLabelOnScreen;

	Texture * mGameOverLabel;
	float mGameOverDelay;
	float mGameOverTimer;
	float mGameOverLabelOnScreen;

	LevelStates mCurrentState;

private:
	void StartStage();
	void HandleCollisions();
	void HandlePlayerDeath();

public:
	Level(Player * player);
	~Level();

	LevelStates State();

	void Update() override;
	void Render() override;
};
#endif