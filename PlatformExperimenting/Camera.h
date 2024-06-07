#pragma once
#include "MathHelper.h"
#include "GameEntity.h"
#include "iostream"
#include "Timer.h"

//								*****Important*****
// Camera should *ALWAYS* have a target if we want it our camera to move

namespace SDLFramework {
	class Camera {
	public:
		//GLGraphics.cpp handles DIRECT_FOLLOW movement with projectionMatrix
		//We handle WITHIN_BOUNDS movement

		enum CameraModes {
			DIRECT_FOLLOW,
			WITHIN_BOUNDS
		};

	private:
		static Camera* sInstance;

		CameraModes mCurrentMode = DIRECT_FOLLOW;

		float mX;
		float mY;
		float mMoveSpeed;
		float mZoom;
		float mSmoothingFactor;
		float mZoomLerpFactor;
		
		bool mIsTargetingAnEntity;

		Vector2 mXTargetBounds;
		Vector2 mYTargetBounds;

		GameEntity* mTarget;
		Timer* mTimer;

		void HandleCameraMovement();
	public:
		static Camera* Instance();
		static void Release();

		Camera();
		~Camera();

		void Mode(CameraModes mode);
		CameraModes Mode() const;

		Vector2 Position();
		void Position(Vector2 position);
		void Position(float x, float y);

		float MoveSpeed() const;
		void MoveSpeed(float speed);

		GameEntity* Target();
		void Target(GameEntity* target);
		void RemoveTarget();
		void PositionToCurrentTarget();

		float Zoom() const;
		void HardZoom(float zoom); //Hard Zoom Effect (zooms immediately)
		void SmoothZoom(float zoom); //Smooth Zoom Effect (zooms over time)
		void AdjustCoordinates(float& x, float& y) const;

		bool IsTargetingAnEntity() const;

		void Update();
	};
}