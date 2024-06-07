#include "MapManager.h"
#include <iostream>

MapManager::MapManager() {}

MapManager::~MapManager() {}

bool MapManager::loadMap(const std::string& filePath)
{
    if (map.load(filePath))
    {
        std::cout << "Map loaded successfully." << std::endl;
        return true;
    }
    else
    {
        std::cerr << "Failed to load the map." << std::endl;
        return false;
    }
}

std::vector<tmx::TileLayer::Tile> MapManager::getTiles() const
{
    std::vector<tmx::TileLayer::Tile> tiles;
    for (const auto& layer : map.getLayers())
    {
        if (layer->getType() == tmx::Layer::Type::Tile)
        {
            const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
            tiles.insert(tiles.end(), tileLayer.getTiles().begin(), tileLayer.getTiles().end());
        }
    }
    return tiles;
}
