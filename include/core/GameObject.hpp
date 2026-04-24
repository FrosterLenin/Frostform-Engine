#pragma once
#include "utility/FVector2.hpp"
#include <memory>
#include "core/Collider.hpp"
#include "core/enums/DrawLayers.hpp"
class Game;

class GameObject{
protected:
    FVector2 _Position;
    FVector2 _Velocity;
    FVector2 _Size;
    float _AccelerationIndex;
    bool _Active;
    Color _Color;
    //std::weak_ptr<Game> Game; // GameObjects does not posses it and need to avoid memory leak
    std::shared_ptr<Collider> _Collider;
    Game* _Game;
    DrawLayer _DrawLayer;
public:
    GameObject(Game* game, const FVector2 position = (.0f, .0f), const FVector2 size = (1.f, 1.f));
    GameObject(Game* game, const FVector2 position, const FVector2 size, const Color color, float accelerationIndex);
    GameObject(Game* game, const FVector2 position, const FVector2 size, const Color color);
    GameObject(Game* game, const FVector2 position, const Color color);
    
    
    virtual ~GameObject();

    // Pure virtual methods so the class is abstract
    // So this class is non instaciable
    virtual void Update(float deltaTime) = 0;
    virtual void Draw() = 0;
    virtual void Start() = 0;

    virtual bool CheckCollision(const GameObject& other) const;
    virtual void OnCollisionEnter(FCollisionInfo& collisionInfo);
    virtual void OnCollisionExit(FCollisionInfo& collisionInfo);
    virtual void OnCollisionStay(FCollisionInfo& collisionInfo);
    virtual void DrawCenter() const;
    virtual float GetHalfWidth() const;
    virtual float GetHalfHeight() const;
    virtual void UpdateWithInput(float deltaTime);
    virtual void UpdateAutomatic(float deltaTime);

    void SetPosition(const FVector2& other);
    void SetVelocity(const FVector2& other);
    void SetActive(const bool other);

    FVector2 GetPosition() const;
    FVector2 GetVelocity() const;
    FVector2 GetSize() const;
    FVector2 GetCenter() const;
    Collider* GetCollider() const;
    bool IsActive() const;
    DrawLayer GetDrawLayer() const;

    void Destroy();
   
};