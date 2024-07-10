#ifndef __SCREENMANAGER_H
#define __SCREENMANAGER_H
#include "StartScreen.h"
#include "PlayScreen.h"
#include "MapManager.h"


class ScreenManager {
private:
	static ScreenManager * sInstance;

	enum Screens { Start, Play };
	Screens mCurrentScreen;

	InputManager * mInput;
	MapManager* mMapManager;

	StartScreen * mStartScreen;
	PlayScreen * mPlayScreen;

public:
	static ScreenManager * Instance();
	static void Release();

	void Update();
	void Render();

private:
	ScreenManager();
	~ScreenManager();
};
#endif