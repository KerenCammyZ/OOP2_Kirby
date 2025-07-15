// main.cpp
#include "GameController.h"

int main()
{
	GameController gameController;
	gameController.run();
		
	// Clear resources when the game ends
	ResourceManager::getInstance().clear();
	return 0;
}
