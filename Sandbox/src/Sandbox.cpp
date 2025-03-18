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

Iconoclast::Application *Iconoclast::CreateApplication()
{
    return new Sandbox;
}
