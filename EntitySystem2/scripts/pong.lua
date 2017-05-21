
local table = import('table')

local states = {
	menu, table, score
}

local gameState = {
	state = states.table
}
function onStarted(event)
	print("Started")

	if gameState.state == states.table then
		table.init()
	end
	print("Init finished")
end

subscribe(Events.Started, onStarted)