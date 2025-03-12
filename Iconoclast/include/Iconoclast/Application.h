#pragma once

#include "Core.h"

namespace Iconoclast {

class ICONOCLAST_API Application
{
public:
    Application();
    virtual ~Application();

    virtual void Run();
};

} // namespace Iconoclast
