#include "rasterizerDemo/RasterizerDemoScene.hpp"

#include "core/Background.hpp"
#include "core/Game.hpp"
#include "core/enums/InputActions.hpp"
#include "rasterizerDemo/RasterizerModeUI.hpp"
#include "rasterizerDemo/RasterizerDemo.hpp"
#include "rasterizerDemo/RasterizerTriangleObject.hpp"

RasterizerDemoScene::RasterizerDemoScene(Game* game)
    : Scene(game), _Background(nullptr), _Triangle(nullptr), _ModeUI(nullptr), _LastMode(3)
{
}

void RasterizerDemoScene::Init()
{
    _State = SceneState::ACTIVE;

    RasterizerDemo* demoGame = dynamic_cast<RasterizerDemo*>(_Game);
    if (demoGame)
        demoGame->SetGameScene(this);

    _Game->GetInputManager()->BindKey(InputAction::RASTER_MODE_1, KEY_ONE);
    _Game->GetInputManager()->BindKey(InputAction::RASTER_MODE_2, KEY_TWO);
    _Game->GetInputManager()->BindKey(InputAction::RASTER_MODE_3, KEY_THREE);

    std::weak_ptr<Background> background = SpawnGameObject<Background>(_Game, FVector2{0.0f, 0.0f}, BLACK);
    _Background = background.lock().get();

    std::weak_ptr<RasterizerTriangleObject> triangle = SpawnGameObject<RasterizerTriangleObject>(_Game);
    _Triangle = triangle.lock().get();

    std::weak_ptr<RasterizerModeUI> modeUI = SpawnGameObject<RasterizerModeUI>(_Game, FVector2{10.0f, 10.0f}, FVector2{20.0f, 20.0f}, WHITE);
    _ModeUI = modeUI.lock().get();

    if (_ModeUI) {
        _Game->GetUIManager()->Bind(modeUI.lock());
        _Game->GetUIManager()->BindEvent<int>(_ModeUI, _ModeUI->UpdateEvent =
            [this](int mode) {
                if (_ModeUI)
                    _ModeUI->SetMode(mode);
            });
    }
}

SceneResult RasterizerDemoScene::Update(float deltaTime)
{
    _Game->GetInputManager()->Update();

    for (std::shared_ptr<GameObject>& gameObject : _GameObjects) {
        if (!gameObject->IsActive()) continue;
        gameObject->Update(deltaTime);
    }

    _Game->GetCollisionManager()->Update();
    _Game->GetUIManager()->Update(deltaTime);

    if (_Triangle) {
        int currentMode = _Triangle->GetMode();
        if (currentMode != _LastMode) {
            _LastMode = currentMode;
            _Game->GetUIManager()->TriggerAllEvents<int>(currentMode);
        }
    }

    return SceneResult::CONTINUE;
}

void RasterizerDemoScene::Draw()
{
    BeginDrawing();
    ClearBackground(_Game->GetClearColor());
    _Game->GetDrawManager()->BeginDraw();

    EndDrawing();
}

void RasterizerDemoScene::Destroy()
{
    _State = SceneState::UNLOADING;
    _Background = nullptr;
    _Triangle = nullptr;
    _ModeUI = nullptr;
    _GameObjects.clear();
}
