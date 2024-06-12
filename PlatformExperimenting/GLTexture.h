#ifndef __GLTEXTURE_H
#define __GLTEXTURE_H
#include "Texture.h"

namespace SDLFramework {

	class GLTexture
		: public Texture {
	public:
		GLuint ID;
		GLuint WrapS, WrapT;
		GLuint FilterMag, FilterMin;

		int Mode;
		bool Rendered;
		bool ShouldScroll;
		void* Data;

		SDL_Surface* Surface;

		GLTexture(std::string filename, bool shouldScroll = true, bool managed = false);
		GLTexture(std::string filename, int x, int y, int w, int h, bool shouldScroll = true, bool managed = false);
		GLTexture(std::string text, std::string fontPath, int size, SDL_Color color, bool shouldScroll = true, bool managed = false);
		GLTexture();
		virtual ~GLTexture();

		void Generate();
		void Bind();

		void SetSurfaceTexture(std::string filename, bool managed = false);
		void SetSurfaceTextTexture(std::string text, std::string filename, int size, SDL_Color color, bool managed = false);

		// Inherited from Texture
		virtual void Render() override;
	};
}
#endif
