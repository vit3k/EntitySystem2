function subscribe(event, func)
	internal_subscribe(event, func)
end

function subscribeForObject(event, func, obj)
	internal_subscribeForObject(event, func, obj)
end

Events = {
	Started = internal_Events.Started
}
setmetatable(Events, {__index = internal_Events})

Components = {}
setmetatable(Components, {__index = internal_Components})

function createEntity(definition)
	return internal_createEntity(definition)
end

function registerSystem(system)
	return internal_registerSystem(system)
end

function emit(event, data)
	internal_emit(event, data)
end

function clearWorld()
	internal_clearWorld()
end
