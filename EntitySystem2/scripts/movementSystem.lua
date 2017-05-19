return {
	requiredComponents = { Components.Transform, Components.Velocity },
	processEntity = function(self, entity, deltaTime)
		transform = entity:get(Components.Transform)
		velocity = entity:get(Components.Velocity)

		transform.position.x = transform.position.x + velocity.velocity.x * deltaTime:asSeconds()
		transform.position.y = transform.position.y + velocity.velocity.y * deltaTime:asSeconds()
		
	end
}