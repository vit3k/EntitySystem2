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
