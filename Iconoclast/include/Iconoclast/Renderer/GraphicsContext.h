#pragma once

static_assert(true, "");
#pragma warning(push)
#pragma warning(disable : 4820)
#pragma warning(disable : 4514)

namespace Iconoclast {

    struct ContextProps
    {
        bool VSync;
        bool FullScreen;

        ContextProps(const bool vSync = true, const bool fullScreen = false)
            : VSync(vSync), FullScreen(fullScreen)
        {
        }
    };

    class GraphicsContext
    {
    public:
        virtual ~GraphicsContext()
        {
        }

        virtual void            Initialize(const ContextProps &) = 0;
        virtual void            SwapBuffers()                    = 0;
        virtual void            ShutDown()                       = 0;

        virtual unsigned int    GetWidth() const                 = 0;
        virtual unsigned int    GetHeight() const                = 0;

        static GraphicsContext *Create(const ContextProps &props = ContextProps());

    protected:
        bool m_VSync;
        bool m_FullScreen;
    };

} // namespace Iconoclast

#pragma warning(pop)
