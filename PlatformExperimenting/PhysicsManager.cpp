#include "PhysicsManager.h"

PhysicsManager * PhysicsManager::sInstance = nullptr;

PhysicsManager * PhysicsManager::Instance() {
	if (sInstance == nullptr) {
		sInstance = new PhysicsManager();
	}

	return sInstance;
}

void PhysicsManager::Release() {
	delete sInstance;
	sInstance = nullptr;
}

void PhysicsManager::SetLayerCollisionMask(CollisionLayers layer, CollisionFlags flags) {
	mLayerMasks[static_cast<unsigned int>(layer)] = std::bitset<static_cast<unsigned int>(CollisionLayers::MaxLayers)>(static_cast<unsigned int>(flags));
}

unsigned long PhysicsManager::RegisterEntity(PhysEntity * entity, CollisionLayers layer) {
	mCollisionLayers[static_cast<unsigned int>(layer)].push_back(entity);
	mLastId++;
	return mLastId;
}

void PhysicsManager::UnregisterEntity(unsigned long id) {
	bool found = false;

	for (int i = 0; i < static_cast<unsigned int>(CollisionLayers::MaxLayers) && !found; i++) {
		for (int j = 0; j < mCollisionLayers[i].size() && !found; j++) {
			if (mCollisionLayers[i][j]->GetId() == id) {
				mCollisionLayers[i].erase(mCollisionLayers[i].begin() + j);
				found = true;
			}
		}
	}
}

void PhysicsManager::Update() {
    for (unsigned int i = 0; i < static_cast<unsigned int>(CollisionLayers::MaxLayers); i++) {
        for (unsigned int j = 0; j < static_cast<unsigned int>(CollisionLayers::MaxLayers); j++) {
            if (mLayerMasks[i].test(j) && i <= j) {
                bool anyCollision = false;

                for (unsigned int k = 0; k < mCollisionLayers[i].size(); k++) {
                    for (unsigned int l = 0; l < mCollisionLayers[j].size(); l++) {
                        if (mCollisionLayers[i][k]->CheckCollision(mCollisionLayers[j][l])) {
                            mCollisionLayers[i][k]->Hit(mCollisionLayers[j][l]);
                            mCollisionLayers[j][l]->Hit(mCollisionLayers[i][k]);

                            mCollisionLayers[i][k]->SetIsColliding(true);
                            mCollisionLayers[j][l]->SetIsColliding(true);

                            anyCollision = true;
                        }
                    }
                }

                if (!anyCollision) {
                    for (unsigned int k = 0; k < mCollisionLayers[i].size(); k++) {
                        mCollisionLayers[i][k]->SetIsColliding(false);
                    }
                    for (unsigned int l = 0; l < mCollisionLayers[j].size(); l++) {
                        mCollisionLayers[j][l]->SetIsColliding(false);
                    }
                }
            }
        }
    }
}


PhysicsManager::PhysicsManager() {
	mLastId = 0;

	for (unsigned int i = 0; i < static_cast<unsigned int>(CollisionLayers::MaxLayers); i++) {
		mLayerMasks[i] = std::bitset<static_cast<unsigned int>(CollisionLayers::MaxLayers)>(static_cast<unsigned int>(CollisionFlags::None));
	}
}

PhysicsManager::~PhysicsManager() {
	for (auto l : mCollisionLayers) {
		l.clear();
	}
}
