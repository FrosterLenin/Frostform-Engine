#include "core/Background.hpp"
#include "core/Game.hpp"
#include "raylib.h"
#include <filesystem>

Background::Background(Game* game, const FVector2 position, Color color)
    : GameObject(game, position, color)
    , _Mode(BackgroundMode::COLOR)
    , _PicturePath()
    , _PictureTexture{}
    , _HasPictureTexture(false) {
    _DrawLayer = DrawLayer::BACKGROUND;
}

Background::Background(Game* game, const FVector2 position, const std::string& picturePath, Color clearColor)
    : GameObject(game, position, clearColor)
    , _Mode(BackgroundMode::PICTURE)
    , _PicturePath(picturePath)
    , _PictureTexture{}
    , _HasPictureTexture(false) {
    _DrawLayer = DrawLayer::BACKGROUND;
}

Background::~Background() {
    UnloadPictureTexture();
}

void Background::SetColorMode(Color color) {
    _Mode = BackgroundMode::COLOR;
    _Color = color;
    UnloadPictureTexture();
    ApplyClearColor();
}

bool Background::SetPictureMode(const std::string& picturePath, Color clearColor) {
    _Mode = BackgroundMode::PICTURE;
    _Color = clearColor;
    _PicturePath = picturePath;

    bool loaded = LoadTextureFromPath(_PicturePath);
    ApplyClearColor();
    return loaded;
}

BackgroundMode Background::GetMode() const {
    return _Mode;
}

void Background::Update(float deltaTime){
    ApplyClearColor();
}

void Background::Draw(){
    if (_Mode != BackgroundMode::PICTURE || !_HasPictureTexture || !_Game)
        return;

    FVector2 screenSize = _Game->GetScreenSize();
    Rectangle source = {0.0f, 0.0f, static_cast<float>(_PictureTexture.width), static_cast<float>(_PictureTexture.height)};
    Rectangle destination = {_Position.x, _Position.y, screenSize.x, screenSize.y};
    DrawTexturePro(_PictureTexture, source, destination, {0.0f, 0.0f}, 0.0f, WHITE);
}

void Background::Start(){
    if (_Mode == BackgroundMode::PICTURE)
        LoadTextureFromPath(_PicturePath);
    ApplyClearColor();

}

void Background::ApplyClearColor() const {
    if (_Game)
        _Game->SetClearColor(_Color);
}

bool Background::LoadTextureFromPath(const std::string& picturePath) {
    // Unload any existing texture before loading a new one to prevent memory leaks
    UnloadPictureTexture();

    if (picturePath.empty())
        return false;

    Image image = {};
    bool imageLoaded = false;
    std::string resolvedPath;

    // We attempt to resolve the image path by checking if it's absolute or relative
    const std::filesystem::path requestedPath = std::filesystem::path(picturePath);
    // If the path is absolute, we try to load it directly
    const std::filesystem::path workingDir = std::filesystem::current_path();


    // Function to attempt loading the image from a given path
    // used for both absolute and relative loading
    auto tryLoad = [&](const std::filesystem::path& candidatePath) {
        if (candidatePath.empty() || !std::filesystem::exists(candidatePath))
            return false;

        // Attempt to load the image from the candidate path
        image = LoadImage(candidatePath.string().c_str());
        if (!IsImageValid(image))
            return false;

        resolvedPath = candidatePath.string();
        return true;
    };

    // If the requested path is absolute, we try to load it directly
    if (requestedPath.is_absolute()) {
        imageLoaded = tryLoad(requestedPath);
    } 
    // If the requested path is relative
    // we attempt to resolve it by searching through the working directory and its parents
    else {
        for (std::filesystem::path searchDir = workingDir;; searchDir = searchDir.parent_path()) {
            if (tryLoad(searchDir / requestedPath)) {
                imageLoaded = true;
                break;
            }
            // If we've reached the root directory without finding the image, we stop searching
            if (!searchDir.has_parent_path() || searchDir == searchDir.parent_path())
                break;
        }
    }

    if (!imageLoaded) {
        TraceLog(LOG_ERROR, "Failed to load background image: %s (cwd: %s)", picturePath.c_str(), GetWorkingDirectory());
        return false;
    }

    _PicturePath = resolvedPath;

    // If the image was loaded successfully, we create a texture from it
    // and unload the image from CPU memory 
    // since we no longer need it after creating the texture
    _PictureTexture = LoadTextureFromImage(image);
    UnloadImage(image);

    _HasPictureTexture = _PictureTexture.id != 0;
    if (!_HasPictureTexture)
        TraceLog(LOG_ERROR, "Failed to create background texture: %s", picturePath.c_str());
    return _HasPictureTexture;
}

void Background::UnloadPictureTexture() {
    if (_HasPictureTexture) {
        UnloadTexture(_PictureTexture);
        _PictureTexture = {};
        _HasPictureTexture = false;
    }

}