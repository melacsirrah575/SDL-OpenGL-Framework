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
		void SetTarget(GameEntity* target);
		void PositionToCurrentTarget();

		float GetZoom();
		void SetHardZoom(float zoom); //Hard Zoom Effect
		void SetSmoothZoom(float zoom); //Smooth Zoom Effect
		void AdjustCoordinates(float& x, float& y);

		void Update();
	};
}