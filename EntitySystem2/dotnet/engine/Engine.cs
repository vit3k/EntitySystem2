using System.Runtime.InteropServices;

namespace engine;

[StructLayout(LayoutKind.Sequential)]
public struct ComponentTypeIds
{
    public int Transform;
    public int Text;
    public int Render;
    public int Collision;
    public int Physics;
    public int Velocity;
    public int Sprite;   
}

public class Engine
{
    private Engine() {}
    public static Engine Instance { get; } = new Engine();

    IList<System> systems = new List<System>();

    public void AddSystem(System system)
    {
        systems.Add(system);
    }
    public delegate void ProcessSystemsDelegate();
    public void ProcessSystems(Time deltaTime)
    {
        foreach (var system in systems)
        {
            system.Process(deltaTime);
        }
    }

    public void Init(Configuration config)
    {
        init(config);
    }
    
    public static ComponentTypeIds ComponentTypeIds;
    static Engine() {
        NativeLibrary.SetDllImportResolver(typeof(Engine).Assembly, 
            (name, _, paths) => name == "engine" ? 
                NativeLibrary.GetMainProgramHandle() : IntPtr.Zero);
        ComponentTypeIds = getComponentTypeIds();
    }
    
    public delegate void InitDelegate();
    
    [DllImport("engine", CharSet = CharSet.Unicode, SetLastError = true)]
    private static extern void init(Configuration config);
    
    [DllImport("engine")]
    private static extern void registerSystem(IntPtr systemPtr);

    [DllImport(("engine"))]
    private static extern void subscribe(int eventId, EventDispatcher.ListenerDelegate callback);
    
    [DllImport(("engine"))]
    private static extern ComponentTypeIds getComponentTypeIds();
}

/*
[StructLayout(LayoutKind.Sequential, Size = 16)]
public unsafe struct SharedPtr<T>
{
    public T* Value;
}*/