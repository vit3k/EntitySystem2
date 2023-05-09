using System.Runtime.InteropServices;

namespace engine;

public unsafe class SpriteComponent : IComponent
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Internal
    {
        private readonly IntPtr vtable;
        public Vector2 Scale;
    }

    public IntPtr Instance { get; set; }
    public int Id => Engine.ComponentTypeIds.Sprite;

    public static SpriteComponent Create(string path)
    {
        return new SpriteComponent(path);
    }

    public SpriteComponent()
    {
    }

    public SpriteComponent(string path)
    {
        Instance = spriteComponent_Create(path);
        Console.WriteLine("native created {0}", Instance);
    }
    
    public Vector2 Scale
    {
        get => ((Internal*)Instance.ToPointer())->Scale;
        set
        {
            Console.WriteLine("set scale");
            ((Internal*)Instance.ToPointer())->Scale = value;
        }
    }
    
    [DllImport("engine")]
    private static extern IntPtr spriteComponent_Create(string path);
}