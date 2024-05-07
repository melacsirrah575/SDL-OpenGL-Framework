#ifndef __PLAYSCREEN_H
#define __PLAYSCREEN_H
#include "Level.h"
#include "Platform.h"

class PlayScreen : public GameEntity {
private:
	Timer * mTimer;
	AudioManager * mAudio;

	bool mGameStarted;

	Level * mLevel;
	bool mLevelStarted;

	Player * mPlayer;
	Platform* mMiddlePlatform;

private:
	void StartNextLevel();

public:
	PlayScreen();
	~PlayScreen();

	void StartNewGame();

	bool GameOver();

	void Update() override;
	void Render() override;
};
#endif