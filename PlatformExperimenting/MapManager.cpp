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

    MapManager::MapManager() : tileTexture(nullptr)
    {
        // Initialize GLTexture and set up rendering if needed
    }

    MapManager::~MapManager()
    {
        delete tileTexture;
    }

    bool MapManager::loadMap(const std::string& filePath)
    {
        if (map.load(filePath))
        {
            std::cout << "Map loaded successfully." << std::endl;
            // Load tile texture using GLTexture
            std::string fullPath = SDL_GetBasePath();
            fullPath.append("Assets/Tiled/TestMap.tmx");
            tileTexture = new GLTexture();

            tileTexture->SetSurfaceTexture(fullPath, false); // Adjust path as needed
            return true;
        }
        else
        {
            std::cerr << "Failed to load the map." << std::endl;
            return false;
        }
    }

    void MapManager::Render() const
    {
        for (const auto& layer : map.getLayers())
        {
            if (layer->getType() == tmx::Layer::Type::Tile)
            {
                const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
                const auto& tiles = tileLayer.getTiles();
                const auto& tileSize = map.getTileSize();
                const auto& mapSize = map.getTileCount();

                int firstgid = 1; // Assuming firstgid is 1 based on the .tmx file

                for (unsigned y = 0; y < mapSize.y; ++y)
                {
                    for (unsigned x = 0; x < mapSize.x; ++x)
                    {
                        const auto tileIndex = x + y * mapSize.x;
                        const auto tileID = tiles[tileIndex].ID;

                        if (tileID == 0) continue; // Skip empty tiles

                        // Calculate srcRect
                        int tileIndexInTileset = tileID - firstgid;
                        int tilesetColumns = 16; // Assuming the tileset image has 10 columns
                        int srcX = (tileIndexInTileset % tilesetColumns) * tileSize.x;
                        int srcY = (tileIndexInTileset / tilesetColumns) * tileSize.y;
                        SDL_Rect srcRect = { srcX, srcY, static_cast<int>(tileSize.x), static_cast<int>(tileSize.y) };

                        SDL_Rect dstRect = { static_cast<int>(x * tileSize.x), static_cast<int>(y * tileSize.y), static_cast<int>(tileSize.x), static_cast<int>(tileSize.y) };

                        std::cout << "Tile ID: " << tiles[tileIndex].ID << std::endl;
                        GLGraphics::Instance()->DrawSprite(tileTexture, &srcRect, &dstRect, 0.0f, SDL_FLIP_NONE, false);
                    }
                }
            }
        }
    }
}

