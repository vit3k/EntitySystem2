namespace engine;

public class Entity
{
    private IntPtr nativePtr;
    private IDictionary<int, IComponent> components;
    
    public int Id { get; set; }
    public T? Get<T>() where T: class, IComponent, new()
    {
        var id = (new T()).GetId();
        return components[id] as T;
    }

    public bool Has<T>() where T : class, IComponent, new()
    {
        var id = (new T()).GetId();
        return components.ContainsKey(id);
    }

    public static Entity Create(params IComponent[] components)
    {
        var nativePtr = NativeEngine.createEntity();
        var entity = new Entity();
        entity.nativePtr = nativePtr;
        return entity;
    }
}