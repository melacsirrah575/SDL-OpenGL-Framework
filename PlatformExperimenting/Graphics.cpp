#include "Graphics.h"
#include "GLGraphics.h"

namespace SDLFramework {

	Graphics* Graphics::sInstance = nullptr;
	bool Graphics::sInitialized = false;
	Graphics::RenderMode Graphics::sMode = Graphics::RenderMode::SDL;

	void Graphics::SetMode(RenderMode mode) {
		sMode = mode;
	}

	// static member functions
	Graphics* Graphics::Instance() {
		if (sInstance == nullptr) {
			switch (sMode) {
			case RenderMode::SDL:
				sInstance = new Graphics();
				break;
			case RenderMode::GL:
				sInstance = new GLGraphics();
				break;
			default:
				break;
			}
		}

		switch (sMode) {
		case RenderMode::SDL:
			return sInstance;
		case RenderMode::GL:
			return static_cast<GLGraphics*>(sInstance);
		default:
			return nullptr;
		}
	}

	void Graphics::Release() {
		delete sInstance;
		sInstance = nullptr;
		sInitialized = false;
	}

	bool Graphics::Initialized() {
		return sInitialized;
	}

	void Graphics::SetCameraPosition(float x, float y) {
		mCameraX = x;
		mCameraY = y;
		//std::cout << "Camera Position: " << mCameraX << ", " << mCameraY << std::endl;
	}

	SDL_Texture* Graphics::LoadTexture(std::string path) {
		if (mRenderer == nullptr) return nullptr;
		
		SDL_Texture* tex = nullptr;
		SDL_Surface* surface = IMG_Load(path.c_str());

		if (surface == nullptr) {
			std::cerr << "Unable to load " << path << ". IMG Error: " << IMG_GetError() << std::endl;
			return nullptr;
		}

		tex = SDL_CreateTextureFromSurface(mRenderer, surface);
		if (tex == nullptr) {
			std::cerr << "Unable to create texture from surface! IMG Error: " << IMG_GetError() << std::endl;
			return nullptr;
		}

		SDL_FreeSurface(surface);
		return tex;
	}

	SDL_Texture* Graphics::LoadTexture(std::string path, SDL_Color color) {
		if (mRenderer == nullptr) return nullptr;

		SDL_Texture* tex = nullptr;
		SDL_Surface* surface = IMG_Load(path.c_str());

		if (surface == nullptr) {
			std::cerr << "Unable to load " << path << ". IMG Error: " << IMG_GetError() << std::endl;
			return nullptr;
		}


		tex = SDL_CreateTextureFromSurface(mRenderer, surface);
		if (tex == nullptr) {
			std::cerr << "Unable to create texture from surface! IMG Error: " << IMG_GetError() << std::endl;
			return nullptr;
		}

		SDL_SetTextureColorMod(tex, color.r, color.g, color.b);
		SDL_FreeSurface(surface);
		return tex;
	}

	SDL_Surface* Graphics::LoadSurface(std::string path) {
		SDL_Surface* surface = IMG_Load(path.c_str());

		if (surface == nullptr) {
			std::cerr << "Unable to load " << path << ". IMG Error: " << IMG_GetError() << std::endl;
			return nullptr;
		}
		return surface;
	}

	SDL_Surface* Graphics::LoadSurface(std::string path, SDL_Color color) {
		SDL_Surface* surface = IMG_Load(path.c_str());

		if (surface == nullptr) {
			std::cerr << "Unable to load " << path << ". IMG Error: " << IMG_GetError() << std::endl;
			return nullptr;
		}

		//// Lock the surface if needed
		//if (SDL_MUSTLOCK(surface)) {
		//	if (SDL_LockSurface(surface) < 0) {
		//		std::cerr << "Unable to lock surface. SDL Error: " << SDL_GetError() << std::endl;
		//		SDL_FreeSurface(surface);
		//		return nullptr;
		//	}
		//}

		// Get the pixel format information
		SDL_PixelFormat* format = surface->format;

		// Loop through each pixel
		for (int y = 0; y < surface->h; y++) {
			for (int x = 0; x < surface->w; x++) {
				// Get the pixel
				Uint32* pixels = (Uint32*)surface->pixels + y * surface->pitch / 4 + x;

				// Get the RGB values of the pixel
				Uint8 r, g, b;
				SDL_GetRGB(*pixels, format, &r, &g, &b);

				// Modify the RGB values by the color argument
				r = (r * color.r) / 255;
				g = (g * color.g) / 255;
				b = (b * color.b) / 255;

				// Set the modified pixel back
				*pixels = SDL_MapRGB(format, r, g, b);
			}
		}

		// Unlock the surface if needed
		//if (SDL_MUSTLOCK(surface)) {
		//	SDL_UnlockSurface(surface);
		//}

		return surface;
	}

	SDL_Texture* Graphics::CreateTextTexture(TTF_Font* font, std::string text, SDL_Color color) {
		if (mRenderer == nullptr) return nullptr;
		
		SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);

		if (surface == nullptr) {
			std::cerr << "CreateTextTexture:: TTF_RenderText_Solid Error: " << TTF_GetError() << std::endl;
			return nullptr;
		}

		SDL_Texture* tex = SDL_CreateTextureFromSurface(mRenderer, surface);
		if (tex == nullptr) {
			std::cerr << "CreateTextTexture:: SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
			return nullptr;
		}

		SDL_FreeSurface(surface);
		return tex;
	}

	SDL_Surface* Graphics::CreateTextSurface(TTF_Font* font, std::string text, SDL_Color color) {
		SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);

		if (surface == nullptr) {
			std::cerr << "CreateTextTexture:: TTF_RenderText_Blended Error: " << TTF_GetError() << std::endl;
			return nullptr;
		}
		return surface;
	}

	void Graphics::DrawTexture(SDL_Texture* tex, SDL_Rect* srcRect, SDL_Rect* dstRect, float angle, SDL_RendererFlip flip) {
		// Apply camera transformation
		if (mCameraX != 0.0f || mCameraY != 0.0f) {
			SDL_Rect adjustedDstRect = *dstRect;
			adjustedDstRect.x -= static_cast<int>(mCameraX);
			adjustedDstRect.y -= static_cast<int>(mCameraY);
			SDL_RenderCopyEx(mRenderer, tex, srcRect, &adjustedDstRect, angle, nullptr, flip);
		}
		else {
			SDL_RenderCopyEx(mRenderer, tex, srcRect, dstRect, angle, nullptr, flip);
		}
	}

	void Graphics::DrawLine(float startX, float startY, float endX, float endY) {
		SDL_Color color;
		SDL_GetRenderDrawColor(mRenderer, &color.r, &color.g, &color.b, &color.a);
		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

		if (mCameraX != 0.0f || mCameraY != 0.0f) {
			startX -= mCameraX;
			startY -= mCameraY;
			endX -= mCameraX;
			endY -= mCameraY;
		}

		SDL_RenderDrawLine(mRenderer, static_cast<int>(startX), static_cast<int>(startY), static_cast<int>(endX), static_cast<int>(endY));
		SDL_SetRenderDrawColor(mRenderer, color.r, color.g, color.b, color.a);
	}

	void Graphics::ClearBackBuffer() {
		SDL_RenderClear(mRenderer);
	}
	void Graphics::Render() {
		SDL_RenderPresent(mRenderer);
	}

	//private member functions
	Graphics::Graphics() : mRenderer(nullptr), mCameraX(0.0f), mCameraY(0.0f) {
		sInitialized = Init();
	}

	Graphics::~Graphics() {
		SDL_DestroyRenderer(mRenderer);
		SDL_DestroyWindow(mWindow);

		mRenderer = nullptr;
		mWindow = nullptr;

		TTF_Quit();
		IMG_Quit();
	}

	bool Graphics::Init() {

		mWindow = SDL_CreateWindow(
			WINDOW_TITLE,				// window title
			SDL_WINDOWPOS_UNDEFINED,	// window x pos
			SDL_WINDOWPOS_UNDEFINED,	// window y pos
			SCREEN_WIDTH,				// window width
			SCREEN_HEIGHT,				// window height
			SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);			// window flags
		if (mWindow == nullptr) {
			std::cerr << "Unable to create Window! SDL Error: " << SDL_GetError() << std::endl;
			return false;
		}

		switch (sMode) {
		case RenderMode::SDL:
			std::cout << "Using SDL" << std::endl;
			mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
			if (mRenderer == nullptr) {
				std::cerr << "Unable to create renderer! SDL Error: " << SDL_GetError() << std::endl;
				return false;
			}
			break;
		case RenderMode::GL:
		{
			std::cout << "Using OpenGL" << std::endl;
			mGLContext = SDL_GL_CreateContext(mWindow);
			if (mGLContext == nullptr) {
				std::cerr << "Unable to create GL context! SDL Error: " << SDL_GetError() << std::endl;
				return false;
			}

			GLenum error = glewInit();
			if (error != GLEW_OK) {
				std::cerr << "Unable to initialize GLEW! GLEW Error: " << glewGetErrorString(error) << std::endl;
				return false;
			}

			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
			glClearColor(0.0f, 0.5f, 0.0f, 1.0f);
			glEnable(GL_TEXTURE_2D);
		}
		break;
		default:
			std::cerr << "Graphics::Init:: Invalid render mode." << std::endl;
			return false;
		}

		if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
			std::cerr << "Unable to initialize SDL video! SDL Error: " << SDL_GetError() << std::endl;
			return false;
		}

		int flags = IMG_INIT_PNG;
		if (!(IMG_Init(flags) & flags)) {
			std::cerr << "Unable to initialize SDL_image! IMG Error: " << IMG_GetError() << std::endl;
			return false;
		}

		if (TTF_Init() == -1) {
			std::cerr << "Unable to initialized SDL_ttf! TTF Error: " << TTF_GetError() << std::endl;
			return false;
		}

		return true;
	}
}