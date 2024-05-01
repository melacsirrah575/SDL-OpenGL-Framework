#ifndef __STARTSCREEN_H
#define __STARTSCREEN_H
#include "AnimatedGLTexture.h"
#include "InputManager.h"
#include "Scoreboard.h"

using namespace SDLFramework;

class StartScreen : public GameEntity {

private:
	Timer * mTimer;
	InputManager * mInput;

	// Top Bar Entities
	GameEntity * mTopBar;
	GLTexture * mHiScore;

	// Play Mode Entities
	GameEntity * mPlayModes;
	GLTexture * mOnePlayerMode;
	GLTexture * mTwoPlayerMode;
	GLTexture * mCursor;
	Vector2 mCursorStartPos;
	Vector2 mCursorOffset;
	int mSelectedMode;

public:
	StartScreen();
	~StartScreen();

	int SelectedMode();

	void ChangeSelectedMode(int change);

	void Update() override;
	void Render() override;
};
#endif