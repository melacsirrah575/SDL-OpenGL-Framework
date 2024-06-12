#ifndef __GAMEMANAGER_H
#define __GAMEMANAGER_H
#include "Timer.h"
#include "AudioManager.h"
#include "ScreenManager.h"
#include "PhysicsManager.h"
#include "PlatformManager.h"
#include "Camera.h"
#include "MapManager.h"

namespace SDLFramework {

	class GameManager
	{
	private:
		static GameManager* sInstance;
		const int FRAME_RATE = 60;

		bool mQuit;
		Graphics* mGraphics;
		Camera* mCamera;
		AssetManager* mAssetManager;
		InputManager* mInputManager;
		AudioManager* mAudioManager;
		PhysicsManager* mPhysicsManager;
		PlatformManager* mPlatformManager;
		MapManager* mMapManager;

		Timer* mTimer;

		SDL_Event mEvent;

		ScreenManager* mScreenManager;

	public:
		static GameManager* Instance();
		static void Release();

		void Run();

		void Update();
		void LateUpdate();

		void Render();

		GameManager();
		~GameManager();
	};
}
#endif