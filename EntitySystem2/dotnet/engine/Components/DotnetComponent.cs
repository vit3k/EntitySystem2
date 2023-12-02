using System.Reflection;
using System.Runtime.InteropServices;

namespace engine;

public class DotnetComponent : Component
{
    static DotnetComponent()
    {

    }

    public DotnetComponent()
    {
        Instance = dotnetComponent_Create(Id);
    }

    

    [DllImport("engine")]
    private static extern IntPtr dotnetComponent_Create(int id);
}