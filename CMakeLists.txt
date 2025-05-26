cmake_minimum_required(VERSION 3.31)
project(OpenGL_Minecraft)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -static -static-libgcc -static-libstdc++")

file(GLOB ASSETS "./assets")

file(COPY ${ASSETS} DESTINATION ${CMAKE_BINARY_DIR})

add_definitions(-DDEBUG)

set(GL_Dir D:/CLion_Code/OpenGL_MC/thirdParty) #目录名称
add_executable(OpenGL_MC ${GL_Dir}/src/glad.c main.cpp)
add_compile_definitions(_GLIBCXX_USE_CXX11_ABI=1)

INCLUDE_DIRECTORIES(${GL_Dir}/include) # 头文件和外部库的位置
link_directories(${GL_Dir}/lib)

add_subdirectory(application)
add_subdirectory(wrapper)
add_subdirectory(glframework)
add_subdirectory(imgui)
add_subdirectory(minecraft)

target_link_libraries(OpenGL_MC ${GL_Dir}/lib/glfw3.dll ${GL_Dir}/lib/libassimp-5.dll app wrapper fw mc imgui)