using System.Runtime.InteropServices;

namespace engine;

public class SpriteComponent : NativeComponent
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Internal
    {
        private readonly IntPtr vtable;
        public Vector2 Scale;
    }

    public SpriteComponent()
    {
    }

    public SpriteComponent(string path)
    {
        Instance = spriteComponent_Create(path);
    }
    
    public unsafe Vector2 Scale
    {
        get => ((Internal*)Instance.ToPointer())->Scale;
        set => ((Internal*)Instance.ToPointer())->Scale = value;
    }
    
    [DllImport("engine")]
    private static extern IntPtr spriteComponent_Create(string path);
}