using System.Reflection;
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

[StructLayout(LayoutKind.Sequential)]
public struct EventTypeIds
{
    public int StartedEvent;
    public int EntityCreatedEvent;
    public int EntityRemovedEvent;
    public int ComponentAttachedEvent;
};

[StructLayout(LayoutKind.Sequential)]
public struct EngineDelegates
{
    public Engine.ProcessDelegate Process;
}

public class Engine
{
    private Engine()
    {
    }

    public static Engine Instance { get; } = new();

    IList<System> systems = new List<System>();

    public void AddSystem(System system)
    {
        systems.Add(system);
    }

    public delegate void ProcessDelegate(long deltaTime);

    public void Process(long deltaTime)
    {
        foreach (var system in systems)
        {
            system.Process(new Time(deltaTime));
        }
    }

    public void Init(Configuration config)
    {
        Console.WriteLine("Engine init c#");
        EventDispatcher.Instance.Init();
        init(config, new EngineDelegates()
        {
            Process = Process
        });
    }
    public static EventTypeIds EventTypeIds;
    static Engine()
    {
        Console.WriteLine("Engine static constructor");
        NativeLibrary.SetDllImportResolver(typeof(Engine).Assembly,
            (name, _, paths) => name == "engine" ? NativeLibrary.GetMainProgramHandle() : IntPtr.Zero);
        //ComponentTypeIds = getComponentTypeIds();
        EventTypeIds = getEventTypeIds();
    }

    public delegate void InitDelegate();

    [DllImport("engine", CharSet = CharSet.Unicode, SetLastError = true)]
    private static extern void init(Configuration config, EngineDelegates engineDelegates);

    [DllImport("engine")]
    private static extern void registerSystem(IntPtr systemPtr);

    [DllImport(("engine"))]
    public static extern void subscribe(int eventId, EventDispatcher.ListenerDelegate callback);

    

    [DllImport(("engine"))]
    private static extern EventTypeIds getEventTypeIds();
    
}