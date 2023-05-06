using System.Runtime.InteropServices;

namespace engine;
[StructLayout(LayoutKind.Sequential)]
public abstract class System {
     
    public abstract void ProcessEntity(Entity entity, Time deltaTime);
    public void Process(Time deltaTime) {}
    
}