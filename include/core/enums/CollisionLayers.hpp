#pragma once
#include <cstdint>

// Collision layer bitflags. Each layer is a single bit so layers/masks can be
// combined using bitwise OR (|) to express which layers an object belongs to
// and which layers it can collide with.
// 1u << 0 means 1 shifted left by 0 bits, which is just 1 (00000001 in binary).
// 1u << 1 means 1 shifted left by 1 bit, which is 2 (00000010 in binary) etc...
enum class CollisionLayer : uint32_t {
    NONE = 0
    , DEFAULT = 1u << 0
    , PLAYER = 1u << 1
    , ENEMY = 1u << 2
    , PROJECTILE = 1u << 3
    // Second projectile layer for different "teams" entities, e.g. player projectiles vs enemy projectiles
    , PROJECTILE_2 = 1u << 4 
    , WALL = 1u << 5
    , PICKUP = 1u << 6
    , UI = 1u << 7

    // All bits set to 1
    // meaning it belongs to all layers or can collide with all layers depending on the context.
    , ALL = 0xFFFFFFFFu 
};

// Bitwise operators so CollisionLayer can be combine
// With | it's possible to combine layers/masks, e.g. CollisionLayer::PLAYER | CollisionLayer::ENEMY 
// to be on both layers or collide with both layers.
inline CollisionLayer operator|(CollisionLayer a, CollisionLayer b){
    return static_cast<CollisionLayer>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}
// With & it's possible to check if a layer is part of a combined layer/mask
//e.g. (CollisionLayer::PLAYER | CollisionLayer::ENEMY) & CollisionLayer::PLAYER will be non-zero (true) because PLAYER is part of the combination.
inline CollisionLayer operator&(CollisionLayer a, CollisionLayer b){
    return static_cast<CollisionLayer>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}
// With ~ it's possible to invert a layer/mask
// e.g. ~CollisionLayer::WALL will be all layers except WALL.
inline CollisionLayer operator~(CollisionLayer a){
    return static_cast<CollisionLayer>(~static_cast<uint32_t>(a));
}
// with |= and &= it's possible to combine and assign in one step
// e.g. layer |= CollisionLayer::ENEMY will add the ENEMY layer to the existing layer variable.
inline CollisionLayer& operator|=(CollisionLayer& a, CollisionLayer b){
    a = a | b; return a;
}
inline CollisionLayer& operator&=(CollisionLayer& a, CollisionLayer b){
    a = a & b; return a;
}
// Helper function to check if a specific layer is part of a combined layer/mask
// e.g. HasLayer(CollisionLayer::PLAYER | CollisionLayer::ENEMY, CollisionLayer::PLAYER) 
// will return true because PLAYER is part of the combination
// e.g. HasLayer(colliderA._Mask, colliderB._Layer) 
// to check if A can collide with B based on A's mask and B's layer.
inline bool HasLayer(CollisionLayer value, CollisionLayer flag){
    return (static_cast<uint32_t>(value) & static_cast<uint32_t>(flag)) != 0u;
}
