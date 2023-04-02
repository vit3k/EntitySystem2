cmake_minimum_required(VERSION 3.0.0)
project(Pong VERSION 0.1.0)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

if(NOT DEFINED ${CMAKE_BUILD_TYPE})
    set(CMAKE_BUILD_TYPE Release)
endif()

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
                TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_configure(REQUIRES lua/5.4.4
                      REQUIRES glm/0.9.9.8
                      REQUIRES sol2/3.2.1
                      GENERATORS cmake_find_package)

conan_cmake_autodetect(settings)

conan_cmake_install(PATH_OR_REFERENCE .
                    BUILD missing
                    REMOTE conancenter
                    SETTINGS ${settings})
                    
find_package(SFML 2.5 COMPONENTS graphics system window REQUIRED)
find_package(sol2 3.2 REQUIRED)
find_package(glm 0.9 REQUIRED)
find_package(lua 5.4 REQUIRED)

add_executable(Pong main.cpp Collider.cpp Input.cpp
    CollisionSystem.cpp InputController.cpp PhysicsSystem.cpp RenderSystem.cpp ScriptManager.cpp
    TextRenderingSystem.cpp EntityW/Component.cpp EntityW/Entity.cpp EntityW/EventDispatcher.cpp
    EntityW/System.cpp)
set_property(TARGET Pong PROPERTY CXX_STANDARD 17)

target_link_libraries(Pong lua::lua sol2::sol2 glm::glm sfml-system sfml-window sfml-graphics)
