#pragma once
#include "core/GameObject.hpp"
#include "core/enums/BackgroundMode.hpp"
#include <string>

class Background : public GameObject{
private:
    BackgroundMode _Mode;
    std::string _PicturePath;
    Texture2D _PictureTexture;
    bool _HasPictureTexture;
public:
    Background(Game* game, const FVector2 position, Color color);
    Background(Game* game, const FVector2 position, const std::string& picturePath, Color clearColor = BLACK);
    ~Background() override;

    // Set the background to a solid color mode with the specified color
    void SetColorMode(Color color);
    // Set the background to picture mode with the specified image path and clear color fallback
    // Returns true if the picture was loaded successfully, false otherwise
    // (in which case the background will use the clear color)
    bool SetPictureMode(const std::string& picturePath, Color clearColor = BLACK);
    BackgroundMode GetMode() const;

    virtual void Update(float deltaTime) override;
    virtual void Start() override;
    virtual void Draw() override;

private:
    void ApplyClearColor() const;
    bool LoadTextureFromPath(const std::string& picturePath);
    void UnloadPictureTexture();
};