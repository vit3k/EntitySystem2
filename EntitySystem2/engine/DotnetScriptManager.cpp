#include "DotnetScriptManager.h"

#include <memory>
#include "Configuration.h"
#include "Engine.h"
#include "Input.h"

void* DotnetScriptManager::load_library(const char_t *path)
{
    void *h = dlopen(path, RTLD_LAZY | RTLD_LOCAL);
    assert(h != nullptr);
    return h;
}

void* DotnetScriptManager::get_export(void *h, const char *name)
{
    void *f = dlsym(h, name);
    assert(f != nullptr);
    return f;
}

// Using the nethost library, discover the location of hostfxr and get exports
bool DotnetScriptManager::load_hostfxr()
{
    // Pre-allocate a large buffer for the path to hostfxr
    char_t buffer[MAX_PATH];
    size_t buffer_size = sizeof(buffer) / sizeof(char_t);
    int rc = get_hostfxr_path(buffer, &buffer_size, nullptr);
    if (rc != 0)
        return false;

    // Load hostfxr and get desired exports
    void *lib = load_library(buffer);
    init_fptr = (hostfxr_initialize_for_runtime_config_fn)get_export(lib, "hostfxr_initialize_for_runtime_config");
    get_delegate_fptr = (hostfxr_get_runtime_delegate_fn)get_export(lib, "hostfxr_get_runtime_delegate");
    close_fptr = (hostfxr_close_fn)get_export(lib, "hostfxr_close");

    return (init_fptr && get_delegate_fptr && close_fptr);
}

// Load and initialize .NET Core and get desired function pointer for scenario
load_assembly_and_get_function_pointer_fn DotnetScriptManager::get_dotnet_load_assembly(const char_t *config_path)
{
    // Load .NET Core
    void *load_assembly_and_get_function_pointer = nullptr;
    hostfxr_handle cxt = nullptr;
    int rc = init_fptr(config_path, nullptr, &cxt);
    if (rc != 0 || cxt == nullptr)
    {
        std::cerr << "Init failed: " << std::hex << std::showbase << rc << std::endl;
        close_fptr(cxt);
        return nullptr;
    }

    // Get the load assembly function pointer
    rc = get_delegate_fptr(
        cxt,
        hdt_load_assembly_and_get_function_pointer,
        &load_assembly_and_get_function_pointer);
    if (rc != 0 || load_assembly_and_get_function_pointer == nullptr)
        std::cerr << "Get delegate failed: " << std::hex << std::showbase << rc << std::endl;

    close_fptr(cxt);
    return (load_assembly_and_get_function_pointer_fn)load_assembly_and_get_function_pointer;
}

void DotnetScriptManager::init()
{
    /*char_t host_path[MAX_PATH];
    auto resolved = realpath(argv[0], host_path);
    assert(resolved != nullptr);

    string_t root_path = host_path;
    auto pos = root_path.find_last_of(DIR_SEPARATOR);
    assert(pos != string_t::npos);
    root_path = root_path.substr(0, pos + 1);*/

    //
    // STEP 1: Load HostFxr and get exported hosting functions
    //
    if (!load_hostfxr())
    {
        assert(false && "Failure: load_hostfxr()");
        return;
    }
    //
    // STEP 2: Initialize and start the .NET Core runtime
    //
    const string_t config_path = "dotnet/bin/Debug/dotnet.runtimeconfig.json";
    load_assembly_and_get_function_pointer_fn load_assembly_and_get_function_pointer = nullptr;
    load_assembly_and_get_function_pointer = get_dotnet_load_assembly(config_path.c_str());
    assert(load_assembly_and_get_function_pointer != nullptr && "Failure: get_dotnet_load_assembly()");

    // STEP 3: Load managed assembly and get function pointer to a managed method
    const string_t dotnetlib_path = "dotnet/bin/Debug/dotnet.dll";
    const char_t *dotnet_type = (initClass + ", dotnet").c_str();

    auto rc = load_assembly_and_get_function_pointer(
        dotnetlib_path.c_str(),
        dotnet_type,
        initMethod.c_str() /*method_name*/,
        //UNMANAGEDCALLERSONLY_METHOD,
        STR("engine.Engine+InitDelegate, dotnet") /*delegate_type_name*/,
        nullptr,
        (void**)&native_engine_init);
    assert(rc == 0 && native_engine_init != nullptr && "Failure: load_assembly_and_get_function_pointer()");

    native_engine_init();
}

void DotnetScriptManager::process(EntityW::Time deltaTime)
{

}

void DotnetScriptManager::close()
{

}

#define ENGINE_API extern "C" __attribute__((visibility("default")))

ENGINE_API void init(Configuration config) {
    Engine::getInstance()->init(config);
}

ENGINE_API void input_mapAction(const char* action, sf::Keyboard::Key key) {
    Input::mapAction(action, key);
}

ENGINE_API bool input_isActionPressed(const char* action) {
    return Input::isActionPressed(action);
}

ENGINE_API void subscribe(EntityW::TypeId eventId, EntityW::native_engine_event_callback callback )
{
    EntityW::EventDispatcher::get().subscribe(eventId, callback);
}

ENGINE_API ComponentTypeIds getComponentTypeIds()
{
    return ComponentTypeIds {
        EntityW::ComponentTypeId<TransformComponent>(),
        EntityW::ComponentTypeId<TextComponent>(),
        EntityW::ComponentTypeId<RenderComponent>(),
        EntityW::ComponentTypeId<CollisionComponent>(),
        EntityW::ComponentTypeId<PhysicsComponent>(),
        EntityW::ComponentTypeId<VelocityComponent>(),
        EntityW::ComponentTypeId<SpriteComponent>()
    };
}

ENGINE_API EntityW::Entity* entity_Create()
{
    auto entity = EntityW::Entity::create();
    return entity.get();
}

ENGINE_API int entity_GetId(EntityW::Entity* entity)
{
    return entity->id;
}

TransformComponent* transformComponent;
ENGINE_API void entity_Attach(EntityW::Entity* entity, EntityW::BaseComponent* component)
{
    entity->attach(EntityW::ComponentSp(component));
}

ENGINE_API TransformComponent* transformComponent_Create(Vector2 position)
{
    return new TransformComponent(position);
}

ENGINE_API EntityW::BaseComponent* entity_GetComponent(EntityW::Entity* entity, int componentId)
{
    printf("Get componentid %d\n", componentId);
    return entity->get(componentId).get();
}

ENGINE_API SpriteComponent* spriteComponent_Create(char* path)
{
    auto sprite = new SpriteComponent(path);
    sprite->load();
    return sprite;
}

ENGINE_API void entity_Commit(EntityW::Entity* entity)
{
    entity->commit();
}

ENGINE_API bool entity_Has(EntityW::Entity* entity, int componentId)
{
    return entity->has(componentId);
}