#include "MapManager.h"
#include <iostream>

namespace SDLFramework {
    MapManager* MapManager::sInstance = nullptr;

    MapManager* MapManager::Instance() {
        if (sInstance == nullptr) {
            sInstance = new MapManager();
        }

        return sInstance;
    }

    void MapManager::Release() {
        delete sInstance;
        sInstance = nullptr;
    }

    MapManager::MapManager()
    {
        // Initialize GLTexture and set up rendering if needed
    }

    MapManager::~MapManager()
    {
        for (auto texture : tileTextures)
        {
            delete texture;
        }
    }

    //CURRENT ISSUE AT HAND
    //We are trying to set the WHOLE map as a SINGLE Texture
    //We want to instead loop through the map and make each tile its own Texture
    bool MapManager::LoadMap(const std::string& filePath)
    {
        std::string fullPath = SDL_GetBasePath();
        fullPath.append("Assets/Tiled/" + filePath);

        if (mMap.load(fullPath))
        {
            std::cout << "Map loaded successfully." << std::endl;

            // Load each tileset in the map
            for (const auto& tileset : mMap.getTilesets())
            {
                LoadTileset(tileset);
            }

            return true;
        }
        else
        {
            std::cerr << "Failed to load the map." << std::endl;
            return false;
        }
    }

    void MapManager::LoadTileset(const tmx::Tileset& tileset) {
        // Extract the image source and load the texture
        const std::string& imagePath = tileset.getName() + ".png";

        GLTexture* texture = new GLTexture();
        texture->SetSurfaceTexture(imagePath, false); // Adjust path if needed

        tileTextures.push_back(texture);
    }

    void MapManager::Render() const
    {
        for (const auto& layer : mMap.getLayers())
        {
            if (layer->getType() == tmx::Layer::Type::Tile)
            {
                const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
                const auto& tiles = tileLayer.getTiles();
                const auto& tileSize = mMap.getTileSize();
                const auto& mapSize = mMap.getTileCount();

                for (unsigned y = 0; y < mapSize.y; ++y)
                {
                    for (unsigned x = 0; x < mapSize.x; ++x)
                    {
                        const auto tileIndex = x + y * mapSize.x;
                        const auto tileID = tiles[tileIndex].ID;

                        if (tileID == 0) continue; // Skip empty tiles

                        // Find the appropriate tileset for this tileID
                        const tmx::Tileset* tileset = nullptr;
                        for (const auto& ts : mMap.getTilesets())
                        {
                            if (tileID >= ts.getFirstGID() && tileID < ts.getFirstGID() + ts.getTileCount())
                            {
                                tileset = &ts;
                                break;
                            }
                        }

                        if (!tileset) continue; // Tile ID not found in any tileset

                        // Calculate srcRect
                        int tileIndexInTileset = tileID - tileset->getFirstGID();
                        int tilesetColumns = tileset->getColumnCount();
                        int srcX = (tileIndexInTileset % tilesetColumns) * tileSize.x;
                        int srcY = (tileIndexInTileset / tilesetColumns) * tileSize.y;
                        SDL_Rect srcRect = { srcX, srcY, static_cast<int>(tileSize.x), static_cast<int>(tileSize.y) };

                        SDL_Rect dstRect = { static_cast<int>(x * tileSize.x), static_cast<int>(y * tileSize.y), static_cast<int>(tileSize.x), static_cast<int>(tileSize.y) };

                        // Find the appropriate texture
                        GLTexture* texture = tileTextures[tileset - &mMap.getTilesets()[0]];
                        GLGraphics::Instance()->DrawSprite(texture, &srcRect, &dstRect, 0.0f, SDL_FLIP_NONE, false);
                    }
                }
            }
        }
    }
}

