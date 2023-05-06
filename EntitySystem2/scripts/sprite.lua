require('engine')

Engine.init({
    window = {
        width = 800,
        height = 600,
        title = 'test'
    },
    frameLimit = 60
})

Input.mapAction('Left', Key.A);
Input.mapAction('Right', Key.D);

registerSystem({
    name = 'InputSystem',
    requiredComponents = { Components.Input, Components.Transform },
    processEntity = function(self, entity, deltaTime)      
        local transform = entity:get(Components.Transform)
        if Input.isActionPressed('Left') then
            transform.position.x = transform.position.x - 10 * deltaTime:asSeconds()
        end
        if Input.isActionPressed('Right') then
            transform.position.x = transform.position.x + 10 * deltaTime:asSeconds()
        end
    end
})

createEntity({
    transform = { x = -9.5, y = 4 },
    input = {},
    sprite = { path = 'assets/player2.png', scale = { x = 0.3, y = 0.3 } }
})
