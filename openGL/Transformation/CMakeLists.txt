cmake_minimum_required(VERSION 2.8.0)
project (GL_chapter8)

set(PROJECTS_LINK_LIBS glfw GLEW GL GLU)
link_directories( /usr/lib)

set(CMAKE_CXX_FLAGS "-Wall -std=c++11 -lpthread -lGL -lGLEW -lglfw -lGLU")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include/glm-1.0.1)


file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# set(SOURCES)

add_executable(GL_Transformation ${SOURCES})
target_link_libraries(GL_Transformation ${PROJECTS_LINK_LIBS})
