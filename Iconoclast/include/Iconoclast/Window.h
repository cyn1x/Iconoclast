#pragma once

#include "Core.h"
#include <string>

static_assert(true, "");
#pragma warning(push)
#pragma warning(disable : 4514)

namespace Iconoclast {

    struct WindowProps
    {
        std::string  Title;
        unsigned int Width;
        unsigned int Height;

        WindowProps(const std::string &title = "Iconoclast Engine", unsigned int width = 1280,
                    unsigned int height = 720)
            : Title(title), Width(width), Height(height)
        {
        }
    };

    // Interface representing a desktop system based window
    class ICONOCLAST_API Window
    {
    public:

        virtual ~Window()
        {
        }

        virtual void         OnUpdate()        = 0;

        virtual unsigned int GetWidth() const  = 0;
        virtual unsigned int GetHeight() const = 0;

        // Window attributes
        virtual void   SetVSync(bool enabled)                            = 0;
        virtual bool   IsVSync() const                                   = 0;

        static Window *Create(const WindowProps &props = WindowProps());
    };

} // namespace Iconoclast

#pragma warning(pop)
