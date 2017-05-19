local scoreManager = require 'scripts/scoreManager'
local AttachSystem = import('attachSystem')
local MovementSystem = import('movementSystem')

local InputSystem = import('inputSystem')

function init()
	-- clearWorld() needs to be added when changing states will be available

	inputSystem = registerSystem(InputSystem)
	movementSystem = registerSystem(MovementSystem)
	attachSystem = registerSystem(AttachSystem)


--[[	scoreText = createEntity({
		transform = { x = -9.5, y = -7.0 },
		text = "0 - 0"
	})
--]]
	--Paddle for player 1
	paddle1 = createEntity({
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
--[[	paddle2 = createEntity({
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

	--Ball
	ball = createEntity({
		transform = { x = -0.15, y = -5 },
		render = {
			shape = { type = Shapes.Circle, radius = 0.3, color = sfColor.White }
		},
		velocity = {
			velocity = { x = -8, y = 4 },
			bounciness = 1
		},
		collision = {
			shape = { type = Shapes.Circle, radius = 0.3 }
		},
		physics = { bounciness = 1, mass = 1,
			constraints = { x = 1, y= 1 }
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
	--]]
	scoreManager.init(scoreText, ball)

end

return {
	init = init
}