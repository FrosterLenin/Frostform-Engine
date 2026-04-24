#pragma once
#include <memory>
#include <vector>
#include "core/GameObject.hpp"

struct DrawableLayer{
    std::vector<std::shared_ptr<GameObject>> Drawables;
};

class DrawManager{
private:
    std::vector<DrawableLayer> _Layer;
public:
    DrawManager();
    ~DrawManager();

    void Init();

    void BeginDraw() const;
    void EndDraw() const;

    void BindObject(std::shared_ptr<GameObject> gameObject);
    size_t ToIndex(DrawLayer layer) const;
};