#pragma once

enum class DrawLayer{
// keep the order of these layers in mind when drawing, as sometimes they will be drawn in 
// this order (lower layers will be drawn first, and higher layers will be drawn on top)
    BACKGROUND
    , GAME_FIELD
    , UI
    
    , LAST
};