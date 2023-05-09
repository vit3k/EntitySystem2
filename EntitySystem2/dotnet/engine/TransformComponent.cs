using System.Runtime.InteropServices;

namespace engine;

public unsafe class TransformComponent : IComponent
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Internal
    {
        private readonly IntPtr vtable;
        public Vector2 Position;
    }
    
    public IntPtr Instance { get; set; }

    internal TransformComponent(TransformComponent.Internal* native)
        : this(new IntPtr(native))
    {
    }

    internal TransformComponent(IntPtr native)
    {
        Instance = native;
    }

    public TransformComponent()
    {
    }

    public TransformComponent(Vector2 position)
    {
        Instance = transformComponent_Create(position);
    }

    public Vector2 Position
    {
        get => ((Internal*)Instance.ToPointer())->Position;
        set => ((Internal*)Instance.ToPointer())->Position = value;
    }
    public int Id => Engine.ComponentTypeIds.Transform;

    public static unsafe TransformComponent Create(Vector2 position)
    {
        return new TransformComponent(position);
    }
    
    [DllImport("engine")]
    private static extern unsafe IntPtr transformComponent_Create(Vector2 position);
}