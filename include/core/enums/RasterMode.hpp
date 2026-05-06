#pragma once

// Algorithm selector for the unified Rasterizer dispatcher.
enum class RasterMode
{
    DDA_LINE            // Two-point line (DDA).
    , BBOX_TRIANGLE     // Flat-color triangle via bounding-box scan.
    , SCANLINE_TRIANGLE // Per-pixel interpolated triangle with Phong lighting.
};
