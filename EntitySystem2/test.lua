entity = createEntity( {
	transform = Vector2.new(-9.5, -7),
	text = "0 - 0"
})

print(entity.id)

transform = entity:get(Components.Transform)
print(transform)

function onCollision(event)
	print(event.entity1:has(Components.Transform))
end

subscribe(Events.Collision, onCollision)