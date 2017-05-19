return {
	requiredComponents = { Components.Transform, Components.Attach },
	processEntity = function(self, entity, deltaTime)
		transform = entity:get(Components.Transform)
		attach = entity:get(Components.Attach)
		transform.position.x = attach.parentTransform.position.x + attach.relativePosition.x
		transform.position.y = attach.parentTransform.position.y + attach.relativePosition.y
	end
}