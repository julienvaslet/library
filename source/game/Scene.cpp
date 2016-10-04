#include <game/Scene.h>

namespace game
{
	Scene::Scene() : running(true), nextScene(NULL)
	{
	}
	
	Scene::~Scene()
	{
	}
	
	bool Scene::isRunning() const
	{
		return this->running;
	}
	
	Scene * Scene::getNextScene()
	{
		return this->nextScene;
	}
}

