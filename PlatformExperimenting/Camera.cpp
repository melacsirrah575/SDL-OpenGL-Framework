#include "Camera.h"

namespace SDLFramework {
	Camera* Camera::sInstance = nullptr;

	Camera* Camera::Instance() {
		if (sInstance == nullptr) {
			sInstance = new Camera();
		}

		return sInstance;
	}

	void Camera::Release() {
		delete sInstance;
		sInstance = nullptr;
	}

	Camera::Camera() {
		mTimer = Timer::Instance();
		mX = 0.0f;
		mY = 0.0f;
		mMoveSpeed = 300.0f;
		mZoom = 1.0f;
		mSmoothingFactor = 1.0f;
		mZoomLerpFactor = 0.1f;
		mTarget = nullptr;
	}

	Camera::~Camera() {
		delete mTarget;
		mTarget = nullptr;
	}

	Vector2 Camera::GetCameraPosition() {
		return (mX, mY);
	}

	void Camera::SetCameraPosition(Vector2 position) {
		mX = position.x;
		mY = position.y;
	}

	void Camera::SetCameraPosition(float x, float y) {
		mX = x;
		mY = y;
	}

	float Camera::GetMoveSpeed() {
		return mMoveSpeed;
	}

	void Camera::SetMoveSpeed(float moveSpeed) {
		mMoveSpeed = moveSpeed;
	}

	GameEntity* Camera::GetTarget() {
		return mTarget;
	}

	void Camera::SetTarget(GameEntity* target) {
		mTarget = target;
	}

	void Camera::PositionToCurrentTarget() {
		//We want to make sure we have a target before trying to position
		if (mTarget) {
			mX = mTarget->Position().x;
			mY = mTarget->Position().y;
		}
		else {
			std::cout << "Current Target is Null in Camera! Cannot Position to target." << std::endl;
		}
	}

	float Camera::GetZoom() {
		return mZoom;
	}

	void Camera::SetHardZoom(float zoom) {
		mZoom = zoom;
		//Wanting to ensure that the smooth zoom doesn't override the hard zoom
		mSmoothingFactor = zoom;
	}

	void Camera::SetSmoothZoom(float zoom) {
		mSmoothingFactor = zoom;
	}

	void Camera::AdjustCoordinates(float& x, float& y) {
		x = (x - mX) * mZoom;
		y = (y - mY) * mZoom;
	}

	void Camera::Update() {
		if (std::fabs(mZoom - mSmoothingFactor) > 0.01f) {
			mZoom += (mSmoothingFactor - mZoom) * mZoomLerpFactor * mTimer->DeltaTime();
			if (std::fabs(mSmoothingFactor - mZoom) < 0.01f) {
				mZoom = mSmoothingFactor;
			}
		}
	}
}
