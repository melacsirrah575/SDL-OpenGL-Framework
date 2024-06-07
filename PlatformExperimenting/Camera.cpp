#include "Camera.h"
#include "Graphics.h"

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
		mZoomLerpFactor = 1.0f;

		mXTargetBounds = Vector2(Graphics::SCREEN_WIDTH * 0.1, Graphics::SCREEN_WIDTH * 0.9);
		mYTargetBounds = Vector2(Graphics::SCREEN_HEIGHT * 0.1, Graphics::SCREEN_HEIGHT * 0.9);

		mIsTargetingAnEntity = false;
		mTarget = nullptr;
	}

	Camera::~Camera() {
		mTarget = nullptr;
	}

	void Camera::Mode(CameraMode mode) {
		mCurrentMode = mode;
	}

	Camera::CameraMode Camera::Mode() {
		return mCurrentMode;
	}

	Vector2 Camera::Position() {
		return Vector2(mX, mY);
	}

	void Camera::Position(Vector2 position) {
		mX = position.x;
		mY = position.y;
	}

	void Camera::Position(float x, float y) {
		mX = x;
		mY = y;
	}

	float Camera::MoveSpeed() {
		return mMoveSpeed;
	}

	void Camera::MoveSpeed(float moveSpeed) {
		mMoveSpeed = moveSpeed;
	}

	GameEntity* Camera::Target() {
		return mTarget;
	}

	void Camera::Target(GameEntity* target) {
		mTarget = target;
		mIsTargetingAnEntity = true;
	}

	void Camera::RemoveTarget() {
		mTarget = nullptr;
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

	float Camera::Zoom() {
		return mZoom;
	}

	void Camera::HardZoom(float zoom) {
		mZoom = zoom;
		//Wanting to ensure that the smooth zoom doesn't override the hard zoom
		mSmoothingFactor = zoom;
	}

	void Camera::SmoothZoom(float zoom) {
		mSmoothingFactor = zoom;
	}

	void Camera::AdjustCoordinates(float& x, float& y) {
		x = (x - mX) * mZoom;
		y = (y - mY) * mZoom;
	}

	bool Camera::IsTargetingAnEntity() {
		return mIsTargetingAnEntity;
	}

	void Camera::Update() {
		if (std::fabs(mZoom - mSmoothingFactor) > 0.01f) {
			mZoom += (mSmoothingFactor - mZoom) * mZoomLerpFactor * mTimer->DeltaTime();
			if (std::fabs(mSmoothingFactor - mZoom) < 0.01f) {
				mZoom = mSmoothingFactor;
			}
		}

		if (mTarget == nullptr) {
			mIsTargetingAnEntity = false;
		}

		if (mCurrentMode == CameraMode::WITHIN_BOUNDS) {
			if (mTarget) {
				HandleCameraMovement();
			}
		}
	}

	void Camera::HandleCameraMovement() {
		Vector2 targetPos = mTarget->Position(GameEntity::Space::Local);

		//Checking X-Bounds
		if (targetPos.x <= mXTargetBounds.x) {
			Position(Position().x - mMoveSpeed * mTimer->DeltaTime(), Position().y);

			mXTargetBounds.x -= mMoveSpeed * mTimer->DeltaTime();
			mXTargetBounds.y -= mMoveSpeed * mTimer->DeltaTime();
		}
		else if (targetPos.x > mXTargetBounds.y) {
			Position(Position().x + mMoveSpeed * mTimer->DeltaTime(), Position().y);

			mXTargetBounds.x += mMoveSpeed * mTimer->DeltaTime();
			mXTargetBounds.y += mMoveSpeed * mTimer->DeltaTime();
		}

		//Checking Y-Bounds
		if (targetPos.y < mYTargetBounds.x) {
			Position(Position().x, Position().y - mMoveSpeed * mTimer->DeltaTime());

			mYTargetBounds.x -= mMoveSpeed * mTimer->DeltaTime();
			mYTargetBounds.y -= mMoveSpeed * mTimer->DeltaTime();
		}
		else if (targetPos.y > mXTargetBounds.y) {
			Position(Position().x, Position().y + mMoveSpeed * mTimer->DeltaTime());

			mYTargetBounds.x += mMoveSpeed * mTimer->DeltaTime();
			mYTargetBounds.y += mMoveSpeed * mTimer->DeltaTime();
		}
	}
}
