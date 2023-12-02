using engine;

namespace Sprite;

public class MovementSystem : engine.System
{
    //private const float Speed = 15f;
    public MovementSystem() : base(typeof(TransformComponent), typeof(InputComponent),
        typeof(MovementComponent)) {}
    public override void ProcessEntity(Entity entity, Time deltaTime)
    {
        //Console.WriteLine("Entity {0} {1} processed", entity.Id, entity.Instance);
        var transform = entity.Get<TransformComponent>();
        var movement = entity.Get<MovementComponent>();
        
        if (transform == null || movement == null)
        {
            return;
        }
        var newPosition = transform.Position;
        if (Input.IsActionPressed("Left"))
        {
            newPosition.X -= (movement.Speed * deltaTime.asSeconds());
        }
        if (Input.IsActionPressed("Right"))
        {
            newPosition.X += (movement.Speed * deltaTime.asSeconds());
        }

        transform.Position = newPosition;
    }
}