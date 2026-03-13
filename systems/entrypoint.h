#pragma once
#include "Application.h"
#include "log.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

extern Exodus::Application* CreateApplication();

int main(int argc, char** argv)
{
    Exodus::Log::Init();
    auto app = CreateApplication();
    app->run();
}
