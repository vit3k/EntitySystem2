local stateManager = import('stateManager')

function onStarted(event)
	print("Started")
	stateManager:init()
	print("Init finished")
end

subscribe(Events.Started, onStarted)
