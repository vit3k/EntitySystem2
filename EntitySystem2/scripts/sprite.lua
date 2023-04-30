require('engine')

local sprite = SpriteComponent.new('player2.png')
sprite.scale = Vector2.new(0.5, 0.5)

local entity = createEntityDirect({
    TransformComponent.new(Vector2.new(-9.5, -6)),
    sprite
})

