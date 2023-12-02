using System.Runtime.InteropServices;

namespace engine;

public abstract class NativeComponent : Component
{
    //private static ComponentTypeIds componentTypeIds;
    static NativeComponent()
    {
        
    }

    protected NativeComponent(IntPtr instance) : base(instance)
    {
    }

    protected NativeComponent()
    {
    }

    
}