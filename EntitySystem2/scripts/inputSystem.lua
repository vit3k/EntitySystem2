function init()
	Components.Input = registerComponent("input")

	inputSystem = registerSystem({
		state = {
		},
		processEntity = function(self, entity, deltaTime)
			input = entity:get(Components.Input)
			velocity = entity:get(Components.Velocity)
			entityState = self.state[input.controller]
			if entityState ~= nil then
				if entityState.Up then
					velocity.velocity.y = -10
				elseif entityState.Down then
					velocity.velocity.y = 10
				else
					velocity.velocity.y = 0
				end
			end
			self.state[input.controller] = {}
		end,
		onMoveUp = function(self, event)
			if self.state[event.controller] == nil then
				self.state[event.controller] = {}
			end
			self.state[event.controller].Up = true
		end,
		onMoveDown = function(self, event)
			if self.state[event.controller] == nil then
				self.state[event.controller] = {}
			end
			self.state[event.controller].Down = true
		end

	}, Components.Input, Components.Velocity)

	subscribeForObject(Events.MoveUp, inputSystem.onMoveUp, inputSystem)
	subscribeForObject(Events.MoveDown, inputSystem.onMoveDown, inputSystem)

end

return {
	init = init
}