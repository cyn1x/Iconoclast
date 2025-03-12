#include <Iconoclast.h>

class Sandbox : public Iconoclast::Application
{
public:
    Sandbox()
    {
    }

    ~Sandbox()
    {
    }
};

// Explicitly export CreateApplication from the EXE
extern "C" __declspec(dllexport) Iconoclast::Application *CreateApplication()
{
    return new Sandbox;
}
