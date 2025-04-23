#include "IconoclastPCH.h" // IWYU pragma: export

#include "D3DMesh.h"
#include "GraphicsContext.h"
#include "Mesh.h"

namespace Iconoclast {

    static Mesh *CreateStub(GraphicsContext &)
    {
        // TODO: Handle logging for when function is not assigned
        return nullptr;
    }

    Mesh *CreateD3DMesh(GraphicsContext &context)
    {
        return new D3DMesh(context);
    }

    // TODO: Bind once on init for corresponding graphics platform
    // std::function<Mesh *(GraphicsContext &)> Mesh::Create = CreateStub;
    std::function<Mesh *(GraphicsContext &)> Mesh::Create = CreateD3DMesh;

} // namespace Iconoclast
