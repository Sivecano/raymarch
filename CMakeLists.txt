cmake_minimum_required(VERSION 3.16)
project(sivegl)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib)
include_directories(src)
include_directories(lib/include)
include_directories(lib/glfw/include)

set(glm_DIR lib/glm/cmake/glm) # if necessary
find_package(glm REQUIRED)

set(SOURCE
        main.cpp
        src/glad.c
        src/helpers.cpp
)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE})

# Linking GLFW and OGL
target_link_libraries(${CMAKE_PROJECT_NAME} glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARY})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})
target_link_libraries(${CMAKE_PROJECT_NAME} glm::glm)

file(COPY src/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
file(COPY src/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY src/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
