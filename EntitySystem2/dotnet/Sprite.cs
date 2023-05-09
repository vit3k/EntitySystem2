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
        
        var player = Entity.Create(
            new TransformComponent(new Vector2(-9.5f,4)),
            new SpriteComponent("assets/player2.png")
            {
                Scale = new Vector2(.3f, .3f)
            }
        );
    }
}