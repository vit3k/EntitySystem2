using System.Runtime.InteropServices;

namespace engine;


public class EventDispatcher
{
    public delegate void ListenerDelegate(IntPtr ev);
    
    public static EventDispatcher Instance { get; } = new EventDispatcher();

    public void Subscribe(int eventId, ListenerDelegate callback)
    {
        NativeEngine.subscribe(eventId, callback);
    }
}
public class Event
{
    
}

public class StartedEvent : Event
{
     
}

[StructLayout(LayoutKind.Sequential)]
public struct EntityCreatedEvent
{
    public IntPtr EntityPtr;
    
}