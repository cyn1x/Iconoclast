#pragma once

#include "IconoclastPCH.h"

#include "GraphicsContext.h"

namespace Iconoclast {

    struct Color
    {
        float r, g, b, a;

        Color(float r = 0.5f, float g = 0.5f, float b = 0.5f, float a = 1.0f)
            : r(r), g(g), b(b), a(a)
        {
        }
    };

    struct Vertex
    {
        float x, y, z;
        Color color;
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
