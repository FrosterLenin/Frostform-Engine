#pragma once

#include "utility/FVector3.hpp"
#include "utility/IVector2.hpp"

// Abstract camera base class for the software rasterizer pipeline
// Subclasses implement either perspective or orthographic projection
class ACamera {
public:
    virtual ~ACamera() = default;

    // Projects a world-space point to screen-space pixel coordinates
    virtual IVector2 Project(FVector3 worldPoint) = 0;

    // Transforms a world-space point into camera-relative space
    virtual FVector3 WorldToCameraSpace(FVector3 worldPoint) = 0;

    virtual FVector3 GetPosition() const = 0;
    virtual void SetPosition(FVector3 position) = 0;

    // Backface culling: returns true if the triangle (in camera space) faces away from the viewer 
    // and should be culled (not drawn)
    // A face is a triangle defined by 3 points in camera space
    // If the normal of the triangle is facing away from the camera, it is culled
    virtual bool IsFaceCulled(const FVector3& cp1, const FVector3& cp2, const FVector3& cp3) const = 0;
};
