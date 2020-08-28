#include "Debug.h"

void glClearError() 
{
    while (glGetError() != GL_NO_ERROR); 
}

bool glLogCall(int line, const char* function, const char* file)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error]" << "(" << error << "):" << "\nLine: " << line << "\nFunction: " << function << "\nFile: " << file << "\n";
        return false;
    }
    return true;
}

asddfklasdlkfj