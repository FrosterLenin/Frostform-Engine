#include "core/Scene.hpp"
#include "core/Game.hpp"

Scene::Scene(Game* game) : _Game(game), _State(SceneState::LOADING)
{
}

Scene::~Scene()
{
}

std::shared_ptr<GameObject> Scene::SpawnGameObject(std::shared_ptr<GameObject> gameObject)
{
    return RegisterGameObject(gameObject);
}

std::shared_ptr<GameObject> Scene::RegisterGameObject(std::shared_ptr<GameObject> gameObject)
{
    gameObject->Start();
    _GameObjects.push_back(gameObject);
    
    if (_Game) {
        _Game->GetCollisionManager()->RegisterCollider(gameObject);
        _Game->GetDrawManager()->BindObject(gameObject);
    }
    
    return gameObject;
}

const std::vector<std::shared_ptr<GameObject>>& Scene::GetGameObjects() const
{
    return _GameObjects;
}

SceneState Scene::GetState() const
{
    return _State;
}

Game* Scene::GetGame() const
{
    return _Game;
}

bool Scene::HasNextScene() const
{
    return _NextSceneCreator != nullptr;
}

std::unique_ptr<Scene> Scene::CreateNextScene() const
{
    if (!_NextSceneCreator || !_Game)
        return nullptr;
    return _NextSceneCreator(_Game);
}