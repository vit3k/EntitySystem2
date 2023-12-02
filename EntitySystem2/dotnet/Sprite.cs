using engine;

namespace Sprite;

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
        
        EventDispatcher.Instance.OnStarted += () => Console.WriteLine("Game started");

        Engine.Instance.AddSystem(new MovementSystem());
        
        var player = new Entity(
            new TransformComponent(new Vector2(-9.5f,4)),
            new SpriteComponent("assets/player2.png")
            {
                Scale = new Vector2(.3f, .3f)
            },
            new InputComponent(),
            new MovementComponent()
            {
                Speed = 15.0f
            }
        );
    }
}