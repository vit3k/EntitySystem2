local scoreManager = require('scoreManager')
local AttachSystem = require('attachSystem')
local MovementSystem = require('movementSystem')
local InputSystem = require('inputSystem')
local LaunchBall = require("launchBall")

local table = {}

function table:init()
	-- clearWorld() needs to be added when changing states will be available

	local inputSystem = registerSystem(InputSystem)
	local movementSystem = registerSystem(MovementSystem)
	local attachSystem = registerSystem(AttachSystem)

	local scoreText = createEntity({
			transform = { x = -9.5, y = -7.0 },
			text = "0 - 0"
		})

	--Paddle for player 1
	local paddle1 = createEntity({
		transform = { x = -10, y = -1 },
		render = {
			shape = { type = Shapes.Rectangle, width = 0.5, height = 2, color = sfColor.Green }
		},
		input = { controller = 0 },
		velocity = {
			velocity = { x = 0, y = 0 },
			bounciness = 0
		},
		collision = {
			shape = { type = Shapes.Rectangle, width = 0.5, height = 2 }
		},
		physics = { bounciness = 0, mass = 1,
			constraints = { x = 0, y= 1}
		}
	})

	--Paddle for player 2
	local paddle2 = createEntity({
		transform = { x = 9.5, y = -1 },
		render = {
			shape = { type = Shapes.Rectangle, width = 0.5, height = 2, color = sfColor.Red }
		},
		input = { controller = 1},
		velocity = {
			velocity = { x = 0, y = 0 },
			bounciness = 0
		},
		collision = {
			shape = { type = Shapes.Rectangle, width = 0.5, height = 2 }
		},
		physics = { bounciness = 0, mass = 1,
			constraints = { x = 0, y= 1 }
		}
	})

	local paddle1Transform = paddle1:get(Components.Transform)
	--Ball
	local ball = createEntity({
		transform = { x = -0.15, y = -5 },
		render = {
			shape = { type = Shapes.Circle, radius = 0.3, color = sfColor.White }
		},
		velocity = {
			velocity = { x = 8, y = 1 },
			bounciness = 1
		},
		collision = {
			shape = { type = Shapes.Circle, radius = 0.3 }
		},
		physics = { bounciness = 1, mass = 1,
			constraints = { x = 1, y= 1 }
		},
		attach = {
			parentTransform = paddle1Transform,
			relativePosition = { x = glm.normalize(paddle1Transform.position).x * (-1), y = 0.75 },
			paddle = paddle1
		}
	})

	--Top field border
	local topBorder = createEntity({
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
	local bottomBorder = createEntity({
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
	local paddle1Border = createEntity({
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
			paddle = paddle1,
			launchVelocity = 8
		}
	})

	--Scoring border for player 1
	local paddle2Border = createEntity({
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
			paddle = paddle2,
			launchVelocity = -8
		}
	})
	scoreManager:init(scoreText, ball)
	LaunchBall:init(ball)
end

return table
