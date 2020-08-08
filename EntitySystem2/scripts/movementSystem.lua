return {
	requiredComponents = { Components.Transform, Components.Velocity },
	name = 'MovementSystem',
	processEntity = function(self, entity, deltaTime)
		local transform = entity:get(Components.Transform)
		local velocity = entity:get(Components.Velocity)

		transform.position.x = transform.position.x + velocity.velocity.x * deltaTime:asSeconds()
		transform.position.y = transform.position.y + velocity.velocity.y * deltaTime:asSeconds()

	end
}