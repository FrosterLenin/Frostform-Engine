#pragma once

// State of a scene during its lifecycle
enum class SceneState
{
    LOADING // Scene is initializing
    , ACTIVE // Scene is updating and running
    , PAUSED // Scene is paused
    , TRANSITIONING // Scene is transitioning to another scene
    , UNLOADING // Scene is cleaning up
};
