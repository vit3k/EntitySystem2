using System.Runtime.InteropServices;

namespace engine;



[StructLayout(LayoutKind.Explicit)]
public struct SharedPointer
{
    [FieldOffset(0)]
    public long pointer;
    [FieldOffset(8)]
    public long pointer2;

}
[StructLayout(LayoutKind.Sequential)]
public struct EntityCreatedEvent
{
    public ulong vTable;
    public ulong pointer;
}

public class EventDispatcher
{
    public delegate void StartedDelegate();

    public delegate void EntityCreatedDelegate(Entity entity);

    public event StartedDelegate? OnStarted;
    public event EntityCreatedDelegate? OnEntityCreated;
    public delegate void ListenerDelegate(IntPtr ev);
    
    public static EventDispatcher Instance { get; } = new();

    private void EntityCreated(IntPtr ev)
    {
        if (OnEntityCreated == null) return;
        
        var entityPtr = Marshal.ReadIntPtr(Marshal.ReadIntPtr(ev), 8);
        var entity = Entity.FromNative(entityPtr);
        OnEntityCreated.Invoke(entity);
    }
    public void Init()
    {
        Engine.subscribe(Engine.EventTypeIds.StartedEvent, ev => OnStarted?.Invoke());
        Engine.subscribe(Engine.EventTypeIds.EntityCreatedEvent, EntityCreated);
    }
}