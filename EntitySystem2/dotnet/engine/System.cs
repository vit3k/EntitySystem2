using System.Collections;
using System.Runtime.InteropServices;

namespace engine;

public struct BitMask
{
    private long data;

    public long Data => data;
    public BitMask(long data)
    {
        this.data = data;
    }
    
    public BitMask()
    {
        data = 0;
    }
    public static bool operator ==(BitMask first, BitMask second)
    {
        return first.data == second.data;
    }

    public static bool operator !=(BitMask first, BitMask second)
    {
        return first.data != second.data;
    }
    public bool this[int i]
    {
        get => (data & (1 << i)) != 0;
        set
        {
            if (!value)
            {
                data &= ~(1 << i);
            }
            else
            {
                data |= (1 << i);
            }
        }
    }
}
public abstract class System
{
    private delegate void EntityAddedDelegate(IntPtr entity);
    private IDictionary<long, Entity> entities = new Dictionary<long, Entity>();
    //private Type[] components;
    public IntPtr Instance;
    private BitMask componentsMask = new BitMask();
    //private int[] components;
    
    public string Name => GetType().Name;

    private void EntityAddedCallback(IntPtr entityPtr)
    {
        Console.WriteLine("Entity added callback {0}", entityPtr);
        var entity = Entity.FromNative(entityPtr);
        entities[entity.Id] = entity;
        Console.WriteLine("Dotnet system: Entity {0} {1} added to {2}", entity.Id, entityPtr, Name);
    }
    public System(params Type[] components)
    {
        foreach (var componentType in components)
        {
            var id = Component.GetId(componentType);
            componentsMask[id] = true;
            //components[0]
        }
        // native init
        Instance = system_Create(Name, componentsMask.Data, EntityAddedCallback);
    }
    public abstract void ProcessEntity(Entity entity, Time deltaTime);
    
    public void Process(Time deltaTime)
    {
        foreach(var (_, entity) in entities)
        {
            ProcessEntity(entity, deltaTime);
        }
        //Console.WriteLine("{0} {1}", Name, deltaTime);
    }

    //private void EntityAdded();
    [DllImport("engine")]
    private static extern IntPtr system_Create(string name, long componetsMask, EntityAddedDelegate entityAdded);
    // system_GetEntities
}