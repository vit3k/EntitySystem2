#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <iostream>

// Provided by the AppHost NuGet package and installed as an SDK pack
#include <nethost.h>

// Header files copied from https://github.com/dotnet/core-setup
#include <coreclr_delegates.h>
#include <hostfxr.h>

#include <dlfcn.h>
#include <limits.h>

#include "IScriptManager.h"

#define STR(s) s
#define CH(c) c
#define DIR_SEPARATOR '/'
#define MAX_PATH PATH_MAX

using string_t = std::basic_string<char_t>;

struct lib_args
{
public:
    const char_t *message;
    int number;
    lib_args(const char_t *message, int number) : message(message), number(number) {};    
};

struct ComponentTypeIds {
    int Transform;
    int Text;
    int Render;
    int Collision;
    int Physics;
    int Velocity;
    int Sprite;
};

typedef void (CORECLR_DELEGATE_CALLTYPE *native_engine_init_fn)();

class DotnetScriptManager : public IScriptManager
{
    private:
        hostfxr_initialize_for_runtime_config_fn init_fptr;
        hostfxr_get_runtime_delegate_fn get_delegate_fptr;
        hostfxr_close_fn close_fptr;
        native_engine_init_fn native_engine_init = nullptr;
        void *load_library(const char_t *path);
        void *get_export(void *h, const char *name);
        bool load_hostfxr();
        load_assembly_and_get_function_pointer_fn get_dotnet_load_assembly(const char_t *config_path);
        std::string initClass; 
        std::string initMethod;

    public:
        DotnetScriptManager(std::string initClass, std::string initMethod): initClass(initClass), initMethod(initMethod) {};
        void init();
        void process(EntityW::Time deltaTime);
        void close();
};