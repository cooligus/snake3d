#pragma once

#include <GLFW/glfw3.h>
#include <iostream>

#define ASSERT(x) if (!(x)) __debugbreak();
#define glCheck(x) glClearError();\
    x;\
    ASSERT(glLogCall(__LINE__, #x, __FILE__))

using namespace std;

void glClearError();
bool glLogCall(int line, const char* function, const char* file);