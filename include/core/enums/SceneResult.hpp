#pragma once

// Results that scenes return to indicate what the SceneManager should do next
enum class SceneResult
{
    CONTINUE // Keep running current scene
    , SCENE_COMPLETE // Current scene is done, load next scene
    , RESTART // Restart the current scene
    , EXIT // Exit to main menu or end game
};
