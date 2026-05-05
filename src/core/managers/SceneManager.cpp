#include "core/managers/SceneManager.hpp"
#include "core/Game.hpp"

SceneManager::SceneManager(Game* game) : _Game(game), _CurrentScene(nullptr) {}

SceneManager::~SceneManager()
{
    if (_CurrentScene)
        _CurrentScene->Destroy();
}

SceneResult SceneManager::Update(float deltaTime)
{
    if (!_CurrentScene)
        return SceneResult::EXIT;

    SceneResult result = _CurrentScene->Update(deltaTime);
    
    // Handle SCENE_COMPLETE by loading next scene if available
    if (result == SceneResult::SCENE_COMPLETE) {
        if (_CurrentScene->HasNextScene()) {
            std::unique_ptr<Scene> nextScene = _CurrentScene->CreateNextScene();
            _CurrentScene->Destroy();
            _CurrentScene = std::move(nextScene);
            if (_CurrentScene) {
                ClearAllManagers();  // Clear managers first
                ClearInputBindings();  // Clear input bindings for new scene
                _CurrentScene->Init();
            }
            return SceneResult::CONTINUE;  // Continue with next scene
        } else
            return SceneResult::EXIT; // No next scene, so complete/exit
    }
    return result;
}

void SceneManager::Draw()
{
    if (_CurrentScene)
        _CurrentScene->Draw();
}

Scene* SceneManager::GetCurrentScene() const
{
    return _CurrentScene.get();
}

bool SceneManager::HasActiveScene() const
{
    return _CurrentScene != nullptr;
}

void SceneManager::ClearInputBindings()
{
    if (_Game)
        _Game->GetInputManager()->ClearAllBindings();
}

void SceneManager::ClearAllManagers()
{
    if (_Game)
        _Game->ClearManagers();
}
