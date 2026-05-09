#include "rasterizerDemo/RasterizerModeUI.hpp"

RasterizerModeUI::RasterizerModeUI(Game* game, FVector2 position, FVector2 size, Color color)
    : UIObject<RasterizerModeEvent>(game, position, size, color), _Mode(3)
{
}

void RasterizerModeUI::Start()
{
    SetActive(true);
}

void RasterizerModeUI::Update(float deltaTime)
{
}

void RasterizerModeUI::Draw()
{

    DrawText("Rasterizer Demo", static_cast<int>(_Position.x), static_cast<int>(_Position.y), static_cast<int>(_Size.x), WHITE);
    DrawText("[1] Wireframe (DDA)", static_cast<int>(_Position.x), static_cast<int>(_Position.y) + 30, 18, LIGHTGRAY);
    DrawText("[2] Filled (BBox)", static_cast<int>(_Position.x), static_cast<int>(_Position.y) + 52, 18, LIGHTGRAY);
    DrawText("[3] Scanline + Phong", static_cast<int>(_Position.x), static_cast<int>(_Position.y) + 74, 18, LIGHTGRAY);
    DrawText("[4] Scanline + Phong (2-Sided, normal flip)", static_cast<int>(_Position.x), static_cast<int>(_Position.y) + 96, 18, LIGHTGRAY);
    DrawText(TextFormat("Mode: %d", _Mode), static_cast<int>(_Position.x), static_cast<int>(_Position.y) + 122, 18, YELLOW);
}

void RasterizerModeUI::SetMode(int mode)
{
    _Mode = mode;
}
