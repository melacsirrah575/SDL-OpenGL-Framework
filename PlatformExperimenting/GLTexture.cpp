#include "GLGraphics.h"
#include "GLTexture.h"

namespace SDLFramework {

	GLTexture::GLTexture(std::string filename, bool managed)
		: Texture(filename, managed) { 
		SetSurfaceTexture(filename, managed);
		Data = Surface->pixels;
		
		WrapS = GL_CLAMP_TO_BORDER;
		WrapT = GL_CLAMP_TO_BORDER;

		FilterMag = GL_LINEAR;
		FilterMin = GL_LINEAR;

		mWidth = Surface->w;
		mHeight = Surface->h;
	}

	GLTexture::GLTexture(std::string filename, int x, int y, int w, int h, bool managed)
		: Texture(filename, x, y, w, h, managed) { 
		SetSurfaceTexture(filename, managed);
		Data = Surface->pixels;

		WrapS = GL_CLAMP_TO_BORDER;
		WrapT = GL_CLAMP_TO_BORDER;

		FilterMag = GL_LINEAR;
		FilterMin = GL_LINEAR;
	}

	GLTexture::GLTexture(std::string text, std::string fontPath, int size, SDL_Color color, bool managed, 
		bool typingEffect, unsigned int textDelay)
		: Texture(text, fontPath, size, color, managed) { 

		//If we don't want to use the Typing effect, we will just Set the Surface Text Texture here
		if (!typingEffect) {
			SetSurfaceTextTexture(text, fontPath, size, color, managed);

			WrapS = GL_CLAMP_TO_BORDER;
			WrapT = GL_CLAMP_TO_BORDER;

			FilterMag = GL_LINEAR;
			FilterMin = GL_LINEAR;

			mWidth = Surface->w;
			mHeight = Surface->h;
		}
		//Otherwise, initialize and save inputted data for the future when we apply the effect.
		else {
			mTypingText = text;
			mTypingFont = fontPath;
			mTypingSize = size;
			mTypingColor = color;
			mTypingManaged = managed;
			mTypingEffect = typingEffect;
			mTextDelay = textDelay;

			mDisplayIndex = 0;
			mTextFullyDisplayed = false;
			mLastDisplayTime = SDL_GetTicks();
		}
	}

	GLTexture::~GLTexture() {
		AssetManager::Instance()->DestroySurface(Surface);
		Surface = nullptr;
	}

	void GLTexture::Generate() {
		SDL_PixelFormat format = *Surface->format;

		GLint nOfColors = format.BytesPerPixel;
		if (nOfColors == 4) {
			if (format.Rmask == 0x000000FF) {
				Mode = GL_RGBA;
			}
			else {
				Mode = GL_BGRA;
			}
		}
		else if (nOfColors == 3) {
			if (format.Rmask == 0x000000FF) {
				Mode = GL_RGB;
			}
			else {
				Mode = GL_BGR;
			}
		}
		else {
			Mode = GL_RGBA;
		}

		glPixelStorei(GL_UNPACK_ROW_LENGTH, Surface->pitch / Surface->format->BytesPerPixel);
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilterMin);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilterMag);

		glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, Surface->w, Surface->h, 0, Mode, GL_UNSIGNED_BYTE, Data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	void GLTexture::Bind() {
		glBindTexture(GL_TEXTURE_2D, ID);
	}

	void GLTexture::SetSurfaceTexture(std::string filename, bool managed) {
		Surface = AssetManager::Instance()->GetSurface(filename, managed);
		Data = Surface->pixels;
		if (Surface != nullptr) {
			Generate();
		}
		else {
			std::cerr << "Unable to set surface " << filename << " in GLTexture! Surface is null." << std::endl;
		}
	}

	void GLTexture::SetSurfaceTextTexture(std::string text, std::string filename, int size, SDL_Color color, bool managed) {

		Surface = AssetManager::Instance()->GetTextSurface(text, filename, size, color, managed);
		Data = Surface->pixels;
		if (Surface != nullptr) {
			Generate();
		}
		else {
			std::cerr << "Unable to set surface text " << filename << " in GLTexture! Surface is null." << std::endl;
		}
	}

	void GLTexture::UpdateTypingEffect(std::string text, std::string filename, int size, SDL_Color color, bool managed,
		bool typingEffect, unsigned int textDelay) {

		unsigned int currentTime = SDL_GetTicks();

		//Waits for us to reach delay time, then creates Texture with next letter appended.
		if (currentTime - mLastDisplayTime >= mTextDelay) {
			if (mDisplayIndex < text.length()) {
				++mDisplayIndex;
				std::string partialText = text.substr(0, mDisplayIndex);
				AssetManager::Instance()->DestroySurface(Surface);
				Surface = AssetManager::Instance()->GetTextSurface(partialText, filename, size, color, managed);
				Data = Surface->pixels;

				mLastDisplayTime = currentTime;
			}
			else {
				mTextFullyDisplayed = true;
			}
		}
		else {
		//If we are waiting to create the next letter, just display the current progress
			std::string tempText = "";
			if (mDisplayIndex == 0) {
				tempText = " ";
			}
			else {
				tempText = text.substr(0, mDisplayIndex);
			}
			Surface = AssetManager::Instance()->GetTextSurface(tempText, filename, size, color, managed);
			Data = Surface->pixels;
		}

		if (Surface != nullptr) {
			Generate();
		}
		else {
			std::cerr << "Unable to set typing surface text " << filename << " in GLTexture! Surface is null." << std::endl;
		}

		WrapS = GL_CLAMP_TO_BORDER;
		WrapT = GL_CLAMP_TO_BORDER;

		FilterMag = GL_LINEAR;
		FilterMin = GL_LINEAR;

		mWidth = Surface->w;
		mHeight = Surface->h;
	}

	void GLTexture::Render() {
		UpdateDstRect();

		//Apply typing effect while word has not been fully created (if we are using it)
		if (mTypingEffect && !mTextFullyDisplayed) {
			UpdateTypingEffect(mTypingText, mTypingFont, mTypingSize, mTypingColor, mTypingManaged, mTypingEffect, mTextDelay);
		}

		GLGraphics::Instance()->DrawSprite(this, mClipped ? &mSourceRect : nullptr, &mDestinationRect, Rotation(World), Flip);
	}
}