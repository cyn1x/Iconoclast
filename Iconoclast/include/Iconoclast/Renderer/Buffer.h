#pragma once

#include "IconoclastPCH.h"

#include "GraphicsContext.h"

namespace Iconoclast {

    struct Vertex
    {
        float x, y, z;
    };

    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer()
        {
        }

        virtual void Bind(GraphicsContext &) const   = 0;
        virtual void Unbind(GraphicsContext &) const = 0;
    };

    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer()
        {
        }

        virtual void     Bind(GraphicsContext &) const   = 0;
        virtual void     Unbind(GraphicsContext &) const = 0;

        virtual uint32_t GetCount() const                = 0;
    };

} // namespace Iconoclast
