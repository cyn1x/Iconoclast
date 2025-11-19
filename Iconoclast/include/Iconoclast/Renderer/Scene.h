#pragma once

#include "IconoclastPCH.h" // IWYU pragma: export

static_assert(true, "");
#pragma warning(push)
#pragma warning(disable : 4514)

namespace Iconoclast {

    struct SceneData
    {
        float *worldMatrix;
        float *viewMatrix;
        float *projectionMatrix;
    };

    class Scene
    {
    public:
        Scene(SceneData data = SceneData());
        ~Scene();

        static inline SceneData *GetSceneData()
        {
            return s_SceneData.get();
        }

    private:
        static std::unique_ptr<SceneData> s_SceneData;
    };

} // namespace Iconoclast
#pragma warning(pop)
