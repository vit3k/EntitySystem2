cmake_minimum_required(VERSION 3.0.0)
project(Engine VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

#if(NOT DEFINED ${CMAKE_BUILD_TYPE})
#    set(CMAKE_BUILD_TYPE Release)
#endif()

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

add_library(nethost SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(nethost PROPERTIES
  IMPORTED_LOCATION "/usr/local/share/dotnet/packs/Microsoft.NETCore.App.Host.osx-arm64/7.0.5/runtimes/osx-arm64/native/libnethost.dylib"
  INTERFACE_INCLUDE_DIRECTORIES "/usr/local/share/dotnet/packs/Microsoft.NETCore.App.Host.osx-arm64/7.0.5/runtimes/osx-arm64/native"
)

add_executable(Engine main.cpp engine/Collider.cpp engine/Input.cpp
        engine/CollisionSystem.cpp engine/InputController.cpp engine/PhysicsSystem.cpp engine/RenderSystem.cpp engine/ScriptManager.cpp
        engine/TextRenderingSystem.cpp engine/EntityW/Component.cpp engine/EntityW/Entity.cpp engine/EntityW/EventDispatcher.cpp
        engine/EntityW/System.cpp engine/SpriteRenderSystem.cpp engine/Engine.cpp engine/Engine.h engine/DotnetScriptManager.cpp)
#set_property(TARGET Engine PROPERTY CXX_STANDARD 20)

target_link_libraries(Engine lua::lua sol2::sol2 glm::glm sfml-system sfml-window sfml-graphics nethost)
target_include_directories(Engine PRIVATE engine/inc)