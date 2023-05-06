using System.Runtime.InteropServices;

namespace engine;

[StructLayout(LayoutKind.Sequential)]
public struct LibArgs
{
    public IntPtr Message;
    public int Number;
}

[StructLayout(LayoutKind.Sequential)]
public struct ComponentTypeIds
{
    public int Transform;
    public int Text;
    public int Render;
    public int Collision;
    public int Physics;
    public int Velocity;
    public int Sprite;   
}

public static class NativeEngine {
    [DllImport("engine", CharSet = CharSet.Unicode, SetLastError = true)]
    public static extern void test();

    [DllImport("engine", CharSet = CharSet.Unicode, SetLastError = true)]
    public static extern void init(Configuration config);

    [DllImport("engine", CharSet = CharSet.Ansi, SetLastError = true)]
    public static extern void input_mapAction(string action, Key key);
    [DllImport("engine", CharSet = CharSet.Ansi, SetLastError = true)]
    public static extern bool input_isActionPressed(string action);

    [DllImport("engine")]
    public static extern void registerSystem(IntPtr systemPtr);

    [DllImport(("engine"))]
    public static extern void subscribe(int eventId, EventDispatcher.ListenerDelegate callback);
    
    [DllImport(("engine"))]
    public static extern ComponentTypeIds getComponentTypeIds();

    [DllImport("engine")]
    public static extern IntPtr createEntity();
    
    public delegate void InitDelegate();

    public static ComponentTypeIds ComponentTypeIds;
    
    static NativeEngine() {
        NativeLibrary.SetDllImportResolver(typeof(NativeEngine).Assembly, 
            (name, _, paths) => name == "engine" ? 
                NativeLibrary.GetMainProgramHandle() : IntPtr.Zero);
        ComponentTypeIds = getComponentTypeIds();
    }
}
