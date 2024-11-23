cmake_minimum_required(VERSION 3.0)

project(Snake3D)

set(HEADER_FILES
        "src/Debug.h"
        "src/InputHandler.h"
        "src/pca.h"
        "src/Shader.h"
        "src/ShaderProgram.h"
        "src/Something.h"
        "src/VertexArrayObject.h"
        "src/Window.h"
)

set(SOURCE_FILES
        "src/Debug.cpp"
        "src/InputHandler.cpp"
        "src/main.cpp"
        "src/pca.cpp"
        "src/Shader.cpp"
        "src/ShaderProgram.cpp"
        "src/Something.cpp"
        "src/VertexArrayObject.cpp"
        "src/Window.cpp"
)

## Find dependencies
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
#find_package(freetype REQUIRED)
#find_package(imgui REQUIRED)

## Create main executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE
        #freetype
        GL
        glfw
        GLEW::GLEW
        glm::glm
        #imgui::imgui
)

