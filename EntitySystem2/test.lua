movementSystem = registerSystem({
	processEntity = function(self, entity, deltaTime)
		--print(deltaTime)
	end
}, Components.Transform, Components.Velocity)

scoreText = createEntity({
	transform = Vector2.new(-9.5, -7.0),
	text = "0 - 0"
})
--scoreText:attach(Components.Velocity, VelocityComponent.new(Vector2.new(1,1), 0))

paddle1Shape = RectangleShape.new(Vector2f.new(0.5, 2))
paddle1Shape:setFillColor(sfColor.Green)

paddle1 = createEntity({
	transform = Vector2.new(-10, -1),
	render = {
		shape = paddle1Shape
	},
	input = 0,
	velocity = {
		velocity = Vector2.new(0, 0),
		bounciness = 0
	},
	collision = {
		shape = RectCollisionShape.new(0.5, 2)
	},
	physics = {
		bounciness = 0,
		mass = 1,
		constraints = Vector2.new(0, 1)
	}
})

paddle2Shape = RectangleShape.new(Vector2f.new(0.5, 2))
paddle2Shape:setFillColor(sfColor.Red)

paddle2 = createEntity({
	transform = Vector2.new(9.5, -1),
	render = {
		shape = paddle2Shape
	},
	input = 1,
	velocity = {
		velocity = Vector2.new(0, 0),
		bounciness = 0
	},
	collision = {
		shape = RectCollisionShape.new(0.5, 2)
	},
	physics = {
		bounciness = 0,
		mass = 1,
		constraints = Vector2.new(0, 1)
	}
})

ballShape = CircleShape.new(0.3)
ballShape:setFillColor(sfColor.White)

ball = createEntity({
	transform = Vector2.new(-0.15, -5),
	render = {
		shape = ballShape
	},
	velocity = {
		velocity = Vector2.new(-8, 4),
		bounciness = 1
	},
	collision = {
		shape = CircleCollisionShape.new(0.3)
	},
	physics = {
		bounciness = 1,
		mass = 1,
		constraints = Vector2.new(1, 1)
	}
})

topBorder = createEntity({
	transform = Vector2.new(-10, -8.5),
	collision = {
		shape = RectCollisionShape.new(20, 1)
	},
	physics = {
		bounciness = 0,
		mass = 0,
		constraints = Vector2.new(0, 0)
	}
})

bottomBorder = createEntity({
	transform = Vector2.new(-10, 7.5),
	collision = {
		shape = RectCollisionShape.new(20, 1)
	},
	physics = {
		bounciness = 0,
		mass = 0,
		constraints = Vector2.new(0, 0)
	}
})

paddle1Border = createEntity({
	transform = Vector2.new(-11, -7.5),
	collision = {
		shape = RectCollisionShape.new(1, 15)
	},
	scoringSurface = {
		playerId = 1,
		paddle = paddle1
	}
})

paddle2Border = createEntity({
	transform = Vector2.new(10, -7.5),
	collision = {
		shape = RectCollisionShape.new(1, 15)
	},
	scoringSurface = {
		playerId = 0,
		paddle = paddle2
	}
})

score = {
	0, 0
}

function onScoreCollision(event)
	local scoringSurface
	if event.entity1:has(Components.ScoringSurface) then
		scoringSurface = event.entity1
	elseif event.entity2:has(Components.ScoringSurface) then
		scoringSurface = event.entity2
	end

	print(scoringSurface)

	if scoringSurface ~= nil then
		scoring = scoringSurface:get(Components.ScoringSurface)
		print(scoring)
		score[scoring.playerId + 1] = score[scoring.playerId + 1] + 1
		scoreText:get(Components.Text).text = score[1] .. " - " .. score[2]

		paddleTransform = scoring.paddle:get(Components.Transform)
		print(paddleTransform)
		relativePosition = Vector2.new(glm.normalize(paddleTransform.position).x * (-1), 0.75)
		print(relativePosition)
		attach = AttachComponent.new(paddleTransform, relativePosition)
		ball:attach(Components.Attach, attach)
		
	end
end

subscribe(Events.Collision, onScoreCollision)


