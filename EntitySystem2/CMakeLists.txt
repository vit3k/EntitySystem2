cmake_minimum_required(VERSION 3.0.0)
project(Pong VERSION 0.1.0)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                  "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_run(CONANFILE conanfile.txt  # or relative build/conanfile.txt
                BASIC_SETUP CMAKE_TARGETS
                BUILD missing)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()



#set(SFML_DIR "/Users/witek/Projects/sfml/Frameworks/SFML.framework/Resources/CMake")
#find_package(SFML 2.5 COMPONENTS graphics system window REQUIRED)

add_executable(Pong main.cpp Collider.cpp Input.cpp
    CollisionSystem.cpp InputController.cpp PhysicsSystem.cpp RenderSystem.cpp ScriptManager.cpp
    TextRenderingSystem.cpp EntityW/Component.cpp EntityW/Entity.cpp EntityW/EventDispatcher.cpp
    EntityW/System.cpp)
set_property(TARGET Pong PROPERTY CXX_STANDARD 17)
target_compile_definitions(Pong PUBLIC SFML_STATIC)
message("${CONAN_LIBS}")
target_link_libraries(Pong "${CONAN_LIBS}")

