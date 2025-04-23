#pragma once

#include "IconoclastPCH.h" // IWYU pragma: export

static_assert(true, "");
#pragma warning(push)
#pragma warning(disable : 4514)

namespace Iconoclast {

    // TODO: Remove DirectX namespace using statement
    using namespace DirectX;

    // TODO: Abstract XMMATRIX through platform-independent math library
    struct SceneData
    {
        XMMATRIX worldMatrix;
        XMMATRIX viewMatrix;
        XMMATRIX projectionMatrix;
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
