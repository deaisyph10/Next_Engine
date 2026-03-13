#include "Application.h" 
#include "entrypoint.h"
#include "Startup.cpp"

Exodus::Application* CreateApplication()
{
    return new Starter_app();
}