#include "rasterizerDemo/RasterizerDemoScene.hpp"

#include "core/Background.hpp"
#include "core/Game.hpp"
#include "core/enums/InputActions.hpp"
#include "core/rasterizer/PerspectiveCamera.hpp"
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

    // Create camera — PerspectiveCamera
    // FOV of 1.5 gives a nice view of the triangle without too much distortion
    // but you can adjust it with the + and - keys in the demo
    const FVector2 screenSize = _Game->GetScreenSize();
    _Camera = std::make_unique<PerspectiveCamera>(static_cast<int>(screenSize.x), static_cast<int>(screenSize.y), 1.5f);
    _Camera->SetPosition({0.0f, 0.0f, -4.0f});

    // Rasterizer mode keys
    _Game->GetInputManager()->BindKey(InputAction::RASTER_MODE_1, KEY_ONE);
    _Game->GetInputManager()->BindKey(InputAction::RASTER_MODE_2, KEY_TWO);
    _Game->GetInputManager()->BindKey(InputAction::RASTER_MODE_3, KEY_THREE);
    _Game->GetInputManager()->BindKey(InputAction::RASTER_MODE_4, KEY_FOUR);

    // Camera movement keys
    _Game->GetInputManager()->BindKey(InputAction::CAM_UP, KEY_W);
    _Game->GetInputManager()->BindKey(InputAction::CAM_DOWN, KEY_S);
    _Game->GetInputManager()->BindKey(InputAction::CAM_LEFT, KEY_A);
    _Game->GetInputManager()->BindKey(InputAction::CAM_RIGHT, KEY_D);
    _Game->GetInputManager()->BindKey(InputAction::CAM_FORWARD, KEY_E);
    _Game->GetInputManager()->BindKey(InputAction::CAM_BACK, KEY_Q);
    _Game->GetInputManager()->BindKey(InputAction::CAM_FOV_UP, KEY_KP_ADD);
    _Game->GetInputManager()->BindKey(InputAction::CAM_FOV_DOWN, KEY_KP_SUBTRACT);

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

    // Camera movement
    if (_Camera) {
        constexpr float cameraSpeed = 3.0f;
        constexpr float fovChangeSpeed = 1.0f;
        FVector3 pos = _Camera->GetPosition();
        InputManager* input = _Game->GetInputManager();

        if (input->GetActionState(InputAction::CAM_UP))    
            pos.y += cameraSpeed * deltaTime;
        if (input->GetActionState(InputAction::CAM_DOWN))  
            pos.y -= cameraSpeed * deltaTime;
        if (input->GetActionState(InputAction::CAM_LEFT))  
            pos.x -= cameraSpeed * deltaTime;
        if (input->GetActionState(InputAction::CAM_RIGHT)) 
            pos.x += cameraSpeed * deltaTime;
        if (input->GetActionState(InputAction::CAM_FORWARD)) 
            pos.z += cameraSpeed * deltaTime;
        if (input->GetActionState(InputAction::CAM_BACK))    
            pos.z -= cameraSpeed * deltaTime;
        _Camera->SetPosition(pos);

        // FOV adjustment (perspective only)
        PerspectiveCamera* perspective = dynamic_cast<PerspectiveCamera*>(_Camera.get());
        if (perspective) {
            float fov = perspective->GetFov();
            if (input->GetActionState(InputAction::CAM_FOV_UP))   
                fov += fovChangeSpeed * deltaTime;
            if (input->GetActionState(InputAction::CAM_FOV_DOWN)) 
                fov -= fovChangeSpeed * deltaTime;
            if (fov < 0.1f) fov = 0.1f;
            perspective->SetFov(fov);
        }
    }

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

    DrawCameraInfo();

    EndDrawing();
}

void RasterizerDemoScene::DrawCameraInfo()
{
    if (!_Camera) return;

    FVector3 cameraPosition = _Camera->GetPosition();
    PerspectiveCamera* perspective = dynamic_cast<PerspectiveCamera*>(_Camera.get());
    float fov = perspective ? perspective->GetFov() : 0.f;

    const char* cameraPositionText = TextFormat("Cam: %.2f, %.2f, %.2f", cameraPosition.x, cameraPosition.y, cameraPosition.z);
    const char* cameraFovText = TextFormat("FOV: %.2f", fov);

    int screenWidth = static_cast<int>(_Game->GetScreenSize().x);
    int cameraPositionWidth = MeasureText(cameraPositionText, 16);
    int cameraFovWidth = MeasureText(cameraFovText, 16);

    DrawText(cameraPositionText, screenWidth - cameraPositionWidth - 10, 10, 16, WHITE);
    DrawText(cameraFovText, screenWidth - cameraFovWidth - 10, 28, 16, WHITE);
}

void RasterizerDemoScene::Destroy()
{
    _State = SceneState::UNLOADING;
    _Background = nullptr;
    _Triangle = nullptr;
    _ModeUI = nullptr;
    _GameObjects.clear();
}
