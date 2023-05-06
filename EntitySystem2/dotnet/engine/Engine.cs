namespace engine;

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
        NativeEngine.init(config);
    }
}