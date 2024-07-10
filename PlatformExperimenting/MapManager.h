#pragma once

#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <string>
#include <vector>
#include "GLTexture.h"
#include "GLGraphics.h"

namespace SDLFramework {
    class MapManager
    {
    public:
        MapManager();
        ~MapManager();

        bool LoadMap(const std::string& filePath);
        void Render() const;

        static MapManager* Instance();
        static void Release();

    private:
        static MapManager* sInstance;

        tmx::Map mMap;
        std::vector<GLTexture*> tileTextures;

        void LoadTileset(const tmx::Tileset& tileset);
    };
}
