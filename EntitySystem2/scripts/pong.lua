require('engine')

local stateManager = require('stateManager')

local function onStarted()
	print("Started")
	stateManager:init()

	print("Init finished")
end

subscribe(Events.Started, onStarted)

