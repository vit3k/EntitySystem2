using System.IO.Compression;
using System.Reflection;
using System.Runtime.InteropServices;

namespace engine;

public abstract class Component 
{
    protected static IDictionary<Type, int> componentsMap = new Dictionary<Type, int>();

    public int Id
    {
        get;
        private set;
    }
    public IntPtr Instance { get; init; }

    protected Component(IntPtr instance)
    {
        Instance = instance;
    }
    public static int GetId<T>()
    {
        return componentsMap[typeof(T)];
    }

    public static int GetId(Type type)
    {
        return componentsMap[type];
    }

    protected Component()
    {
        Id = componentsMap[GetType()];
    }

    static Component()
    {
        // TODO: refactor this. Base class should know nothing about inherited ones
        var componentTypeIds = getComponentTypeIds();
        //Console.WriteLine(componentTypeIds);
        componentsMap[typeof(TransformComponent)] = componentTypeIds.Transform;
        componentsMap[typeof(SpriteComponent)] = componentTypeIds.Sprite;
        
        var components = Assembly.GetExecutingAssembly()
            .GetTypes().Where(x => typeof(DotnetComponent)
                                       .IsAssignableFrom(x)
                                   && x is { IsInterface: false, IsAbstract: false }
                                   && x.Name != "DotnetComponent").ToList();
        foreach (var component in components)
        {
            componentsMap[component] = registerComponent(component.Name);
        }
    }
    
    [DllImport("engine")]
    private static extern int registerComponent(string name);
    
    [DllImport(("engine"))]
    private static extern ComponentTypeIds getComponentTypeIds();
}