using System.Runtime.InteropServices;

namespace engine;

[StructLayout(LayoutKind.Sequential)]
public struct TransformComponent : IComponent
{
    public Vector2 Position;
    public static int ID = NativeEngine.ComponentTypeIds.Transform;

    public int GetId()
    {
        return ID;
    }
}