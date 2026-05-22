#pragma once

#include "core/GameObject.hpp"
#include "core/rasterizer/Screen.hpp"
#include "core/rasterizer/ACamera.hpp"
#include "utility/FVector3.hpp"
#include "utility/IVector2.hpp"

#include <array>
#include <memory>

// Just a triangle object to demonstrate the rasterizer modes
// it will be drawn with a logic defined in draw function
class RasterizerTriangleObject : public GameObject
{
public:
    explicit RasterizerTriangleObject(Game* game);
    virtual ~RasterizerTriangleObject() = default;

    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;

    int GetMode() const;
    void SetCamera(ACamera* camera);

private:
    std::unique_ptr<Screen> _Screen;
    ACamera* _Camera;
    // these are the vertices of the triangle in world space
    // they will be transformed and projected to screen space in the draw function
    std::array<FVector3, 3> _BaseVerts;
    // for mode 3, we will use the vertex colors for a simple Gouraud shading effect
    std::array<Color, 3> _BaseColors;
    // the mode in which the triangle will be drawn, it will be set by the input manager in the update function
    int _Mode; // 1 = wireframe, 2 = filled, 3 = scanline+phong, 4 = scanline+phong (two-sided)
    // the angle of rotation for the triangle, it will be updated in the update function and used in the draw function to rotate the triangle
    float _Angle; 
};
