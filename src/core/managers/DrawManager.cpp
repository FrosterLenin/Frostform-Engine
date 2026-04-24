#include "core/managers/DrawManager.hpp"

DrawManager::DrawManager() {}
DrawManager::~DrawManager() {}

void DrawManager::BeginDraw() const{
    for(const auto& layer : _Layer){
        for(const auto& drawable : layer.Drawables){
            if(drawable->IsActive())
                drawable->Draw();
        }
    }
}
void DrawManager::EndDraw() const {
}

size_t DrawManager::ToIndex(DrawLayer layer) const{
    return static_cast<size_t>(layer);
}
void DrawManager::Init() {
    _Layer.clear();
    _Layer.resize(ToIndex(DrawLayer::LAST));
}

void DrawManager::BindObject(std::shared_ptr<GameObject> gameObject){
    size_t index = ToIndex(gameObject->GetDrawLayer());
    if(index < 0 || index >= _Layer.size()) return;
    _Layer[index].Drawables.push_back(gameObject);
}

