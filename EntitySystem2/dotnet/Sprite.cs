using System.Runtime.InteropServices;

namespace Sprite;
using engine;


class Game {
    public static void Init() {
        Engine.Instance.Init(new Configuration{
            Window = new WindowConfiguration{
                Width = 800,
                Height = 600,
                Title = "Sprite"
            },
            FrameLimit = 60
        });

        Input.MapAction("Left", Key.A);
        Input.MapAction("Right", Key.D);

        var entity = Entity.Create();
        Console.WriteLine("Created entity {0}", entity.Id);
        /*EventDispatcher.Instance.Subscribe(0, ev =>
        {
            var e = Marshal.PtrToStructure<EntityCreatedEvent>(ev);
            Console.WriteLine("started " + e.EntityPtr);
        });*/
    }
}