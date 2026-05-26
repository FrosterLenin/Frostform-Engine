#include "core/managers/DrawManager.hpp"

DrawManager::DrawManager() {
    // Initialize the draw layers
    Init();
}
DrawManager::~DrawManager() {}

void DrawManager::BeginDraw() const{
    for(const auto& layer : _Layer){
        for(const auto& drawable : layer.Drawables){
            if(drawable->IsActive())
                drawable->Draw();
        }
    }
}

void DrawManager::DrawLayers(DrawLayer startLayer, DrawLayer endLayer) const
{
    size_t startIndex = ToIndex(startLayer);
    size_t endIndex = ToIndex(endLayer);
    if (_Layer.empty()) return;
    if (startIndex >= _Layer.size()) return;
    if (endIndex >= _Layer.size()) endIndex = _Layer.size() - 1;
    // Ensure startIndex is less than or equal to endIndex
    if (startIndex > endIndex) std::swap(startIndex, endIndex);

    for (size_t i = startIndex; i <= endIndex; ++i) {
        for (const auto& drawable : _Layer[i].Drawables) {
            if (drawable->IsActive())
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

