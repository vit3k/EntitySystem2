using System.Runtime.InteropServices;

namespace engine;

public class Entity
{
    public IntPtr Instance;
    //private IDictionary<int, IComponent> components;

    public int Id => entity_GetId(Instance);

    public T Get<T>() where T: class, IComponent, new()
    {
        var component = new T();
        var componentNative = entity_GetComponent(Instance, component.Id);
        component.Instance = componentNative;
        return component;
    }

    public bool Has<T>() where T : class, IComponent, new()
    {
        var id = (new T()).Id;
        return entity_Has(Instance, id);
    }

    public static Entity Create(params IComponent[] components)
    {
        var nativePtr = entity_Create();
        var entity = new Entity
        {
            Instance = nativePtr
        };
        foreach(var component in components)
        {
            entity.Attach(component);
        }
        entity.Commit();
        return entity;
    }

    public void Attach(IComponent component)
    {
        entity_Attach(Instance, component.Instance);
    }

    public void Commit()
    {
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