#include "PlayScreen.h"

void PlayScreen::StartNextLevel() {
	mLevelStarted = true;
	
	delete mLevel;
	mLevel = new Level(mPlayer);
}

PlayScreen::PlayScreen() {
	mTimer = Timer::Instance();
	mAudio = AudioManager::Instance();
	mPlatforms = PlatformManager::Instance();

	mLevel = nullptr;
	mLevelStarted = false;

	mPlayer = nullptr;

	mPlatforms->CreateNewPlatform(true, Vector2(Graphics::SCREEN_WIDTH * 0.5, Graphics::SCREEN_HEIGHT * 0.7f), Vector2(1.6f, 0.2f));
	mPlatforms->CreateNewPlatform(true, Vector2(Graphics::SCREEN_WIDTH * 0.3, Graphics::SCREEN_HEIGHT * 0.6f), Vector2(1.6f, 0.2f));
	mPlatforms->CreateNewPlatform(true, Vector2(Graphics::SCREEN_WIDTH * 0.5, Graphics::SCREEN_HEIGHT * 0.85), Vector2(9.0f, 0.2f));

}

PlayScreen::~PlayScreen() {
	mTimer = nullptr;
	mAudio = nullptr;

	delete mLevel;
	mLevel = nullptr;

	delete mPlayer;
	mPlayer = nullptr;
}

void PlayScreen::StartNewGame() {
	delete mPlayer;
	mPlayer = new Player();
	mPlayer->Parent(this);
	mPlayer->Position(Graphics::SCREEN_WIDTH * 0.4f, Graphics::SCREEN_HEIGHT * 0.8f);
	mPlayer->Active(false);

	mGameStarted = false;
	mLevelStarted = false;
}

bool PlayScreen::GameOver() {
	return !mLevelStarted ? false : (mLevel->State() == Level::GameOver);
}

void PlayScreen::Update() {

	//Temp Solution until you code HOW the game starts
	if (!mGameStarted) {
		mGameStarted = true;
	}

	if (mGameStarted) {
		//Setting StartNextLevel to a Debug key because every game will want to call this differently
		if (InputManager::Instance()->KeyPressed(SDL_SCANCODE_L)) {
			StartNextLevel();
		}
		else {
			if (mLevel == nullptr) {
				StartNextLevel();
			}
			mLevel->Update();
			if (mLevel->State() == Level::Finished) {
				mLevelStarted = false;
			}
		}

		mPlayer->Update();
		mPlatforms->Update();
		//mMiddlePlatform->Update();
	}
}

void PlayScreen::Render() {

	if (mGameStarted) {

		if (mLevelStarted) {
			mLevel->Render();
		}

		mPlayer->Render();
		mPlatforms->Render();
		//mMiddlePlatform->Render();
	}
}
