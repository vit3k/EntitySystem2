
--MovementSystem
movementSystem = registerSystem({
	processEntity = function(self, entity, deltaTime)
		transform = entity:get(Components.Transform)
		velocity = entity:get(Components.Velocity)

		transform.position.x = transform.position.x + velocity.velocity.x * deltaTime:asSeconds()
		transform.position.y = transform.position.y + velocity.velocity.y * deltaTime:asSeconds()
		
	end
}, Components.Transform, Components.Velocity)


attachSystem = registerSystem({
	processEntity = function(self, entity, deltaTime)
		transform = entity:get(Components.Transform)
		attach = entity:get(Components.Attach)
		
		print(attach.parentTransform.position.x)
		print(attach.relativePosition.x)

		transform.position.x = attach.parentTransform.position.x + attach.relativePosition.x
		transform.position.y = attach.parentTransform.position.y + attach.relativePosition.y
	end
}, Components.Transform, Components.Attach)

--Score display 
scoreText = createEntity({
	transform = {
		x = -9.5, 
		y = -7.0
	},
	text = "0 - 0"
})

--Paddle for player 1
paddle1 = createEntity({
	transform = {
		x = -10,
		y = -1
	},
	render = {
		shape = {
			type = Shapes.Rectangle,
			width = 0.5,
			height = 2,
			color = sfColor.Green
		}
	},
	input = 0,
	velocity = {
		velocity = {
			x = 0,
			y = 0
		},
		bounciness = 0
	},
	collision = {
		shape = {
			type = Shapes.Rectangle,
			width = 0.5,
			height = 2
		}
	},
	physics = {
		bounciness = 0,
		mass = 1,
		constraints = {
			x = 0,
			y= 1
		}
	}
})

--Paddle for player 2
paddle2 = createEntity({
	transform = {
		x = 9.5,
		y = -1
	},
	render = {
		shape = {
			type = Shapes.Rectangle,
			width = 0.5,
			height = 2,
			color = sfColor.Red
		}
	},
	input = 1,
	velocity = {
		velocity = {
			x = 0,
			y = 0
		},
		bounciness = 0
	},
	collision = {
		shape = {
			type = Shapes.Rectangle,
			width = 0.5,
			height = 2
		}
	},
	physics = {
		bounciness = 0,
		mass = 1,
		constraints = {
			x = 0,
			y= 1
		}
	}
})

--Ball
ball = createEntity({
	transform = {
		x = -0.15,
		y = -5
	},
	render = {
		shape = {
			type = Shapes.Circle,
			radius = 0.3,
			color = sfColor.White
		}
	},
	velocity = {
		velocity = {
			x = -8,
			y = 4
		},
		bounciness = 1
	},
	collision = {
		shape = {
			type = Shapes.Circle,
			radius = 0.3
		}
	},
	physics = {
		bounciness = 1,
		mass = 1,
		constraints = {
			x = 1,
			y= 1
		}
	}
})

--Top field border
topBorder = createEntity({
	transform = {
		x = -10,
		y = -8.5
	},
	collision = {
		shape = {
			type = Shapes.Rectangle,
			width = 20,
			height = 1
		}
	},
	physics = {
		bounciness = 1,
		mass = 0,
		constraints = {
			x = 0,
			y= 0
		}
	}
})

--Bottom field border
bottomBorder = createEntity({
	transform = {
		x = -10,
		y = 7.5
	},
	collision = {
		shape = {
			type = Shapes.Rectangle,
			width = 20,
			height = 1
		}
	},
	physics = {
		bounciness = 1,
		mass = 0,
		constraints = {
			x = 0,
			y= 0
		}
	}
})

--Scoring border for player 2
paddle1Border = createEntity({
	transform = {
		x = -11,
		y = -7.5
	},
	collision = {
		shape = {
			type = Shapes.Rectangle,
			width = 1,
			height = 15
		}
	},
	scoringSurface = {
		playerId = 1,
		paddle = paddle1
	}
})

--Scoring border for player 1
paddle2Border = createEntity({
	transform = {
		x = 10,
		y = -7.5
	},
	collision = {
		shape = {
			type = Shapes.Rectangle,
			width = 1,
			height = 15
		}
	},
	scoringSurface = {
		playerId = 0,
		paddle = paddle2
	}
})

--Current score
score = {
	0, 0
}

--Collision hanlder for scoring surfaces
--Adds point for player and attach ball to the other
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

--trying to port input system
function onMoveUp(event)
end

function onMoveDown(event)
end