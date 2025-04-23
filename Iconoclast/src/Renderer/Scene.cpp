#include "IconoclastPCH.h" // IWYU pragma: keep

#include "Scene.h"

namespace Iconoclast {

    std::unique_ptr<SceneData> Scene::s_SceneData = nullptr;

    Scene::Scene(SceneData data)
    {
        s_SceneData = std::make_unique<SceneData>(data);
    }

    Scene::~Scene()
    {
    }

} // namespace Iconoclast
