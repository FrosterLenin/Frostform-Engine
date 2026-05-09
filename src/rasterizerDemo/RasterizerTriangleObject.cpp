#include "rasterizerDemo/RasterizerTriangleObject.hpp"

#include "core/Game.hpp"
#include "core/enums/InputActions.hpp"
#include "core/rasterizer/Rasterizer.hpp"
#include "core/rasterizer/Screen.hpp"

RasterizerTriangleObject::RasterizerTriangleObject(Game* game)
    : GameObject(game, FVector2{0.0f, 0.0f}, FVector2{1.0f, 1.0f}, WHITE),
      _Screen(nullptr),
      _BaseVerts({{{0.0f, 0.9f, 0.0f}, {-0.9f, -0.7f, 0.0f}, {0.9f, -0.7f, 0.0f}}}),
      _BaseColors({{RED, GREEN, BLUE}}),
      _Mode(3),
      _Angle(0.0f)
{
    _DrawLayer = DrawLayer::GAME_FIELD;
}

void RasterizerTriangleObject::Start()
{
    const FVector2 screenSize = _Game->GetScreenSize();
    _Screen = std::make_unique<Screen>(static_cast<int>(screenSize.x), static_cast<int>(screenSize.y));
}

void RasterizerTriangleObject::Update(float deltaTime)
{
    InputManager* inputManager = _Game->GetInputManager();
    if (inputManager->GetActionDown(InputAction::RASTER_MODE_1)) _Mode = 1;
    if (inputManager->GetActionDown(InputAction::RASTER_MODE_2)) _Mode = 2;
    if (inputManager->GetActionDown(InputAction::RASTER_MODE_3)) _Mode = 3;
    if (inputManager->GetActionDown(InputAction::RASTER_MODE_4)) _Mode = 4;

    _Angle += 30.0f * deltaTime;
}

void RasterizerTriangleObject::Draw()
{
    if (!_Screen)
        return;

    std::array<FVector3, 3> world;
    for (int i = 0; i < 3; ++i)
        world[i] = _BaseVerts[i].RotateY(_Angle);

    const FVector3 edge1 = world[1] - world[0];
    const FVector3 edge2 = world[2] - world[0];
    FVector3 normal = edge1.Cross(edge2);
    normal.Normalize();

    std::array<IVector2, 3> screenPosition;
    std::array<float, 3> zPosition;
    for (int i = 0; i < 3; ++i) {
        screenPosition[i] = Project(world[i]);
        zPosition[i] = -world[i].z;
    }

    _Screen->Clear(BLACK);

    if (_Mode == 1)
        Rasterizer::Draw(RasterMode::DDA_LINE, screenPosition[0], screenPosition[1], screenPosition[2], WHITE, _Screen.get());
    else if (_Mode == 2)
        Rasterizer::Draw(RasterMode::BBOX_TRIANGLE, screenPosition[0], screenPosition[1], screenPosition[2], SKYBLUE, _Screen.get());
    else if (_Mode == 3) {
        Gpu gpu;
        gpu.Mode = GpuDrawMode::COLOR;
        gpu.pointLightPosition = {0.0f, 0.0f, -2.0f};
        gpu.cameraPosition = {0.0f, 0.0f, -4.0f};

        std::array<GpuVertex, 3> verts;
        for (int i = 0; i < 3; ++i) {
            verts[i].screenPosition = screenPosition[i];
            verts[i].color = _BaseColors[i];
            verts[i].zPosition = zPosition[i];
            verts[i].uv = {0.0f, 0.0f};
            verts[i].worldPosition = world[i];
            verts[i].worldNormal = normal;
        }

        Rasterizer::SetFaceCullingEnabled(true);
        Rasterizer::SetTwoSidedRenderingEnabled(false);
        Rasterizer::Draw(RasterMode::SCANLINE_TRIANGLE, gpu, verts[0], verts[1], verts[2], _Screen.get());
    } 
    else if (_Mode == 4) {
        Gpu gpu;
        gpu.Mode = GpuDrawMode::COLOR;
        gpu.pointLightPosition = {0.0f, 0.0f, -2.0f};
        gpu.cameraPosition = {0.0f, 0.0f, -4.0f};

        // Check if triangle is backfacing; if so, flip normal for proper lighting
        FVector3 directionToCamera = gpu.cameraPosition - world[0];
        FVector3 normalForRender = normal;
        if (normal.Dot(directionToCamera) < 0.f) 
            normalForRender = normal * -1.0f;

        std::array<GpuVertex, 3> verts;
        for (int i = 0; i < 3; ++i) {
            verts[i].screenPosition = screenPosition[i];
            verts[i].color = _BaseColors[i];
            verts[i].zPosition = zPosition[i];
            verts[i].uv = {0.0f, 0.0f};
            verts[i].worldPosition = world[i];
            verts[i].worldNormal = normalForRender;
        }

        Rasterizer::SetFaceCullingEnabled(false);
        Rasterizer::SetTwoSidedRenderingEnabled(true);
        Rasterizer::Draw(RasterMode::SCANLINE_TRIANGLE, gpu, verts[0], verts[1], verts[2], _Screen.get());
    }

    _Screen->Blit();
}

int RasterizerTriangleObject::GetMode() const
{
    return _Mode;
}

IVector2 RasterizerTriangleObject::Project(const FVector3& point) const
{
    const float fov = 1.5f;
    const float cameraOffset = 4.0f;

    float zPositionOfCam = point.z + cameraOffset;
    if (zPositionOfCam < 0.1f) zPositionOfCam = 0.1f;

    const FVector2 screenSize = _Game->GetScreenSize();
    const float halfW = screenSize.x * 0.5f;
    const float halfH = screenSize.y * 0.5f;

    int sx = static_cast<int>(halfW + (point.x * fov / zPositionOfCam) * halfW);
    int sy = static_cast<int>(halfH - (point.y * fov / zPositionOfCam) * halfH);

    return {sx, sy};
}
