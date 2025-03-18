#pragma once

#include "Application.h"

extern Iconoclast::Application *Iconoclast::CreateApplication();

int                             main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    auto app = Iconoclast::CreateApplication();
    app->Run();
    delete app;
}
