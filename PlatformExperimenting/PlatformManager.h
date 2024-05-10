#pragma once
#include "Platform.h"
#include "map"

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

	void CreateNewPlatform(bool Standable, Vector2 position, GLTexture* texture, Vector2 scale, Vector2 boxColliderOffset = Vec2_Zero);
	void AddPlatformToMap(Platform* platformToAdd);
	
	Platform* GetPlatform(unsigned long ID);
	Vector2 GetPlatformPosition(unsigned long ID);

	void Update();
	void Render();
};