using System.Runtime.InteropServices;

namespace engine;

public class TransformComponent : NativeComponent
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Internal
    {
        private readonly IntPtr vtable;
        public Vector2 Position;
    }

    public TransformComponent(IntPtr native) : base(native)
    {
    }

    public TransformComponent()
    {
    }

    public TransformComponent(Vector2 position)
    {
        Instance = transformComponent_Create(position);
    }

    public unsafe Vector2 Position
    {
        get => ((Internal*)Instance.ToPointer())->Position;
        set => ((Internal*)Instance.ToPointer())->Position = value;
    }

    [DllImport("engine")]
    private static extern IntPtr transformComponent_Create(Vector2 position);
}