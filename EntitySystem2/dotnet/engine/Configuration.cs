using System.Runtime.InteropServices;

namespace engine;

[StructLayout(LayoutKind.Sequential)]
public struct WindowConfiguration {
    public int Width;
    public int Height;
    public string Title;
}
[StructLayout(LayoutKind.Sequential)]
public struct Configuration {
    public WindowConfiguration Window;
    public int FrameLimit;
}