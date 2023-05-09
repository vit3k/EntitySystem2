using System.Runtime.InteropServices;

namespace engine;

[StructLayout(LayoutKind.Sequential)]
public struct Vector2
{
    public float X;
    public float Y;

    public Vector2(float x, float y)
    {
        X = x;
        Y = y;
    }
}
/*
public class Vector2f
{
    private IntPtr nativePtr;

    public Vector2f(IntPtr nativePtr)
    {
        this.nativePtr = nativePtr;
    }

    public Vector2f(float x, float y)
    {
        nativePtr = vector2_New(x, y);
    }
    public float X
    {
        get => vector2_GetX(nativePtr);
        set => vector2_SetX(nativePtr, value);
    }
    public float Y
    {
        get => vector2_GetY(nativePtr);
        set => vector2_SetY(nativePtr, value);
    }
    [DllImport("engine")]
    private static extern float vector2_GetX(IntPtr nativePtr);
    [DllImport("engine")]
    private static extern void vector2_SetX(IntPtr nativePtr, float val);
    [DllImport("engine")]
    private static extern float vector2_GetY(IntPtr nativePtr);
    [DllImport("engine")]
    private static extern void vector2_SetY(IntPtr nativePtr, float val);   
    
    [DllImport("engine")]
    private static extern IntPtr vector2_New(float x, float y);   

}*/
/*
public unsafe class Vector2x : IDisposable
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Internal
    {
        public float X;
        public float Y;
    }
    
    public IntPtr Instance { get; protected set; }

    internal Vector2(Vector2.Internal* native)
        : this(new IntPtr(native))
    {
    }

    internal Vector2(Vector2.Internal native)
        : this(&native)
    {
    }

    internal Vector2(IntPtr native)
    {
        Instance = native;
    }

    public Vector2(float x, float y)
    {
        Instance = Marshal.AllocHGlobal(8);
        X = x;
        Y = y;
    }
    public Vector2()
    {
        Instance = Marshal.AllocHGlobal(8);
    }

    public void Dispose()
    {
        Dispose(disposing: true);
        GC.SuppressFinalize(this);
    }

    protected virtual void Dispose(bool disposing)
    {
        Marshal.FreeHGlobal(Instance);
    }
    
    public float X
    {
        get
        {
            var ptr = (Internal*)Instance.ToPointer();
            return ptr->X;
        }

        set
        {
            var ptr = (Internal*)Instance.ToPointer();
            ptr->X = value;
        }
    }
    
    public float Y
    {
        get
        {
            var ptr = (Internal*)Instance.ToPointer();
            return ptr->Y;
        }

        set
        {
            var ptr = (Internal*)Instance.ToPointer();
            ptr->Y = value;
        }
    }
}*/