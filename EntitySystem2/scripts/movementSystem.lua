function init()
	movementSystem = registerSystem({
		processEntity = function(self, entity, deltaTime)
			transform = entity:get(Components.Transform)
			velocity = entity:get(Components.Velocity)

			transform.position.x = transform.position.x + velocity.velocity.x * deltaTime:asSeconds()
			transform.position.y = transform.position.y + velocity.velocity.y * deltaTime:asSeconds()
		
		end
	}, Components.Transform, Components.Velocity)

end

return {
	init = init
}