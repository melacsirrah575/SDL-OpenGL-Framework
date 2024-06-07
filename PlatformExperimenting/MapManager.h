#pragma once

#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <string>
#include <vector>

class MapManager
{
public:
    MapManager();
    ~MapManager();

    bool loadMap(const std::string& filePath);
    std::vector<tmx::TileLayer::Tile> getTiles() const;

private:
    tmx::Map map;
};