#pragma once
#include "MathHelper.h"
#include "GameEntity.h"
#include "iostream"
#include "Timer.h"

namespace SDLFramework {
	class Camera {
	private:
		static Camera* sInstance;

		float mX;
		float mY;
		float mMoveSpeed;
		float mZoom;
		float mSmoothingFactor;
		float mZoomLerpFactor;
		
		bool mIsTargetingAnEntity;

		GameEntity* mTarget;
		Timer* mTimer;

	public:
		static Camera* Instance();
		static void Release();

		Camera();
		~Camera();

		Vector2 GetCameraPosition();
		void SetCameraPosition(Vector2 position);
		void SetCameraPosition(float x, float y);

		float GetMoveSpeed();
		void SetMoveSpeed(float speed);

		GameEntity* GetTarget();
		//When we call SetTarget, GLGraphics handles moving the camera based on the target's position
		//Otherwise, our Player class is determining when to move the camera
		void SetTarget(GameEntity* target);
		void RemoveTarget();
		void PositionToCurrentTarget();

		float GetZoom();
		void SetHardZoom(float zoom); //Hard Zoom Effect (zooms immediately)
		void SetSmoothZoom(float zoom); //Smooth Zoom Effect (zooms over time)
		void AdjustCoordinates(float& x, float& y);

		bool GetIsTargetingAnEntity();

		void Update();
	};
}