#pragma once
#include "Platform.h"
#include "map"

namespace SDLFramework {
	class PlatformManager {
	private:
		static PlatformManager* sInstance;

		std::map<unsigned long, Platform*> mPlatforms;
		bool mMapHasPlatforms = false;

		PlatformManager() { };
		~PlatformManager();

	public:
		static PlatformManager* Instance();
		static void Release();

		void CreateNewPlatform(bool Standable, bool canJumpThrough, Vector2 position, GLTexture* texture, Vector2 scale = Vec2_One, Vector2 boxColliderSize = Vec2_Zero, Vector2 boxColliderOffset = Vec2_Zero);
		void AddPlatformToMap(Platform* platformToAdd);

		Platform* GetPlatform(unsigned long ID);
		Vector2 GetPlatformPosition(unsigned long ID);

		void Update();
		void Render();
	};
}
