#include "PlatformManager.h"

namespace SDLFramework {
	PlatformManager* PlatformManager::sInstance = nullptr;

	PlatformManager* PlatformManager::Instance() {
		if (sInstance == nullptr) {
			sInstance = new PlatformManager();
		}

		return sInstance;
	}

	void PlatformManager::Release() {
		delete sInstance;
		sInstance = nullptr;
	}

	void PlatformManager::AddPlatformToMap(Platform* platformToAdd) {
		if (mPlatforms.find(platformToAdd->GetId()) != mPlatforms.end()) {
			//We have already added this platform!
			std::cout << "We have already added Platform " << platformToAdd->GetId() << ". Did you mean to GetPlatform?" << std::endl;
			return;
		}

		mPlatforms[platformToAdd->GetId()] = platformToAdd;
		mMapHasPlatforms = true;
	}

	Platform* PlatformManager::GetPlatform(unsigned long ID) {
		if (mPlatforms.find(ID) != mPlatforms.end()) {
			return mPlatforms[ID];
		}
		else {
			std::cerr << "Trying to get a platform that doesn't exist! Returning nullptr" << std::endl;
			return nullptr;
		}
	}

	Vector2 PlatformManager::GetPlatformPosition(unsigned long ID) {
		if (mPlatforms.find(ID) != mPlatforms.end()) {
			return mPlatforms[ID]->Position();
		}
		else {
			std::cerr << "Trying to get a platform that doesn't exist! Returning 0,0" << std::endl;
			return Vec2_Zero;
		}
	}

	void PlatformManager::CreateNewPlatform(bool standable, bool canJumpThrough, Vector2 position, GLTexture* texture, Vector2 scale, Vector2 boxColliderSize, Vector2 boxColliderOffset) {
		Platform* platform = new Platform(standable, canJumpThrough, position, texture, scale, boxColliderSize, boxColliderOffset);
		AddPlatformToMap(platform);
	}

	PlatformManager::~PlatformManager() {
		for (auto it = mPlatforms.begin(); it != mPlatforms.end(); it++) {
			delete it->second;
		}
	}

	void PlatformManager::Update() {
		if (mMapHasPlatforms) {
			for (auto it = mPlatforms.begin(); it != mPlatforms.end(); it++) {
				if (it->second) {
					it->second->Update();
				}
				else {
					std::cerr << "Error: Null pointer in map for key " << it->first << std::endl;
				}
			}
		}
	}

	void PlatformManager::Render() {
		if (mMapHasPlatforms) {
			for (auto it = mPlatforms.begin(); it != mPlatforms.end(); ++it) {
				if (it->second) {
					it->second->Render();
				}
				else {
					std::cerr << "Error: Null pointer in map for key " << it->first << std::endl;
				}
			}
		}
	}
}
