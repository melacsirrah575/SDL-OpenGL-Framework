#include "GameManager.h"
#include "MapManager.h"

using SDLFramework::GameManager;

int main(int argc, char * args[])
{
	GameManager * game = GameManager::Instance();

    MapManager mapManager;
    //std::string fullPath = SDL_GetBasePath();
    //fullPath.append("Assets/Tiled/TestMap.tmx");

    //if (mapManager.loadMap(fullPath))
    //{
    //    auto tiles = mapManager.getTiles();
    //    for (const auto& tile : tiles)
    //    {
    //        std::cout << "Tile ID: " << tile.ID << std::endl;
    //    }
    //}

	game->Run();

	GameManager::Release();
	game = nullptr;

	return 0;
}