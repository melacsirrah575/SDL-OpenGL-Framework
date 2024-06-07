#include "ScreenManager.h"

ScreenManager* ScreenManager::sInstance = nullptr;

ScreenManager* ScreenManager::Instance() {
	if (sInstance == nullptr) {
		sInstance = new ScreenManager();
	}

	return sInstance;
}

void ScreenManager::Release() {
	delete sInstance;
	sInstance = nullptr;
}

void ScreenManager::Update() {

	switch (mCurrentScreen) {
	case Start:
		mStartScreen->Update();

		if (mInput->KeyPressed(SDL_SCANCODE_RETURN)) {
			mCurrentScreen = Play;
			//If our StartScreen has ANY Animations, we should restart them HERE!
			mPlayScreen->StartNewGame();
		}
		break;
	case Play:
		mPlayScreen->Update();

#ifdef _DEBUG
		if (mInput->KeyPressed(SDL_SCANCODE_ESCAPE)) {
			mCurrentScreen = Start;
		}
#endif

		if (mPlayScreen->GameOver()) {
			mCurrentScreen = Start;
		}
		break;
	}
}

void ScreenManager::Render() {

	switch (mCurrentScreen) {
	case Start:
		mStartScreen->Render();
		break;
	case Play:
		mPlayScreen->Render();
		break;
	}
}

ScreenManager::ScreenManager() {
	mInput = InputManager::Instance();

	mStartScreen = new StartScreen();
	mPlayScreen = new PlayScreen();

	mCurrentScreen = Start;
}

ScreenManager::~ScreenManager() {
	mInput = nullptr;

	delete mStartScreen;
	mStartScreen = nullptr;

	delete mPlayScreen;
	mPlayScreen = nullptr;
}