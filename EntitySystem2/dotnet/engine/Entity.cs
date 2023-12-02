using System.Runtime.InteropServices;

namespace engine;

public class Entity
{
    private static IDictionary<IntPtr, Entity> entities = new Dictionary<IntPtr, Entity>();

    public IntPtr Instance;
    private IDictionary<int, Component> dotnetComponents = new Dictionary<int, Component>();

    public int Id
    {
        get;
        init;
    }

    public T? Get<T>() where T: Component, new()
    {
        var tType = typeof(T);
        var id = Component.GetId<T>();
        if (!tType.IsSubclassOf(typeof(NativeComponent))) return dotnetComponents[id] as T;
        
        var nativePtr = entity_GetComponent(Instance, id);
        if (nativePtr == IntPtr.Zero)
        {
            return null;
        }

        return Activator.CreateInstance(tType, nativePtr) as T;
    }

    public bool Has<T>() where T : Component, new()
    {
        var id = Component.GetId<T>();
        return entity_Has(Instance, id);
    }

    public static Entity FromNative(IntPtr native)
    {
        
        if (!entities.ContainsKey(native))
        {
            Console.WriteLine("Entity {0} not found in cache {1}", native, entities.Count);
            entities[native] = new Entity(native);
            return entities[native];
        }
        Console.WriteLine("Entity {0} found in cache {1}", native, entities.Count);
        return entities[native];
    }
    
    public Entity(params Component[] components)
    {
        Instance = entity_Create();
        Id = entity_GetId(Instance);
        foreach(var component in components)
        {
            Attach(component);
        }
        entities[Instance] = this;
        Console.WriteLine("Dotnet constructor: Entity created {0} {1}. Number of entities: {2}", Id, Instance, entities.Count);
        Commit();
    }

    protected Entity(IntPtr native)
    {
        Instance = native;
        Id = entity_GetId(Instance);
        entities[Instance] = this;
        Console.WriteLine("Dotnet constructor from native: Entity created {0} {1}", Id, Instance);
    }

    public void Attach(Component component)
    {
        entity_Attach(Instance, component.Instance);
        if (component is DotnetComponent)
        {
            dotnetComponents[component.Id] = component;
        }
    }

    public void Commit()
    {
        Console.WriteLine("Dotnet: Entity commit");
        entity_Commit(Instance);
    }

    [DllImport("engine")]
    private static extern void entity_Commit(IntPtr instance);
    [DllImport("engine")]
    private static extern IntPtr entity_Create();
    [DllImport("engine")]
    private static extern int entity_GetId(IntPtr entity);
    [DllImport("engine")]
    private static extern IntPtr entity_GetComponent(IntPtr entity, int componentId);
    [DllImport("engine")]
    private static extern void entity_Attach(IntPtr entity, IntPtr component);
    [DllImport("engine")]
    private static extern bool entity_Has(IntPtr entity, int componentId);
}