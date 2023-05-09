namespace engine;

public interface IComponent
{
    public IntPtr Instance { get; set; }
    public int Id { get; }
}