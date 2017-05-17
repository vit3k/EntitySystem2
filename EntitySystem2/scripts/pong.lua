local attachSystem = import('attachSystem')
local movementSystem = import('movementSystem')
local inputSystem = import('inputSystem')
local table = import('table')

states = {
	menu, table, score
}

gameState = {
	state = states.table
}
function onStarted(event)
	print("Started")

	inputSystem.init()
	movementSystem.init()
	attachSystem.init()
	
	if gameState.state == states.table then
		table.init()
	end
	print("Init finished")
	for key, value in pairs(Components) do print(key, ": ", value) end
end

subscribe(Events.Started, onStarted)